//
//  BaseTranscodeOperation.m
//  MetadataTranscoderTestHarness
//
//  Created by vade on 4/4/15.
//  Copyright (c) 2015 Synopsis. All rights reserved.
//

#import "BaseTranscodeOperation.h"

// Pass 1
NSString * const kSynopsisTranscodeVideoSettingsKey = @"kSynopsisVideoTranscodeSettings";
NSString * const kSynopsisTranscodeAudioSettingsKey = @"kSynopsisAudioTranscodeSettings";
NSString * const kSynopsisAnalysisSettingsKey = @"kSynopsisAnalysisSettings";
NSString * const kSynopsisAnalysisSettingsQualityHintKey = @"kSynopsisAnalysisSettingsQualityHintKey";
NSString * const kSynopsisAnalysisSettingsEnableConcurrencyKey = @"kSynopsisAnalysisSettingsEnableConcurrencyKey";
NSString * const kSynopsisAnalysisSettingsEnabledPluginsKey = @"kSynopsisAnalysisSettingsEnabledPluginsKey";
NSString * const kSynopsisAnalysisSettingsEnabledPluginModulesKey = @"kSynopsisAnalysisSettingsEnabledPluginModulesKey";

// Pass 2
NSString * const kSynopsisAnalyzedVideoSampleBufferMetadataKey = @"kSynopsisAnalyzedVideoSampleBufferMetadata";
NSString * const kSynopsisAnalyzedAudioSampleBufferMetadataKey = @"kSynopsisAnalyzedAudioSampleBufferMetadata";
NSString * const kSynopsisAnalyzedGlobalMetadataKey = @"kSynopsisAnalyzedGlobalMetadata";
NSString * const kSynopsisAnalyzedMetadataExportOptionKey = @"kSynopsisAnalyzedMetadataExportOption";

@interface BaseTranscodeOperation ()

@property (atomic, readwrite, strong) id processInfoActivity;

@property (atomic, readwrite, strong) NSURL* sourceURL;
@property (atomic, readwrite, strong) NSURL* destinationURL;

@property (atomic, readwrite, strong) OperationStateWrapper* operationState;
@property (atomic, readwrite, strong) NSDate* startDate;
@property (atomic, readwrite, assign) NSTimeInterval elapsedTime;
@property (atomic, readwrite, assign) NSTimeInterval remainingTime;

@property (atomic, readwrite, assign) BOOL initted;

@property (readwrite, strong) NSError* error;
@property (readwrite, assign) BOOL succeeded;

@end

@implementation BaseTranscodeOperation

@synthesize progress = _progress;
@synthesize audioProgress = _audioProgress;
@synthesize videoProgress = _videoProgress;

- (instancetype) initWithOperationState:(OperationStateWrapper*)operationState sourceURL:(NSURL*)sourceURL destinationURL:(NSURL*)destinationURL;
{
    self = [super init];
    if(self)
    {
        NSLog(@"Alloc Operation %@", [self className]);

        self.error = nil;
        self.initted = NO;
        self.operationState = operationState;
        self.sourceURL = sourceURL;
        self.destinationURL = destinationURL;
        
        self.videoProgress = (CGFloat)0.0;
        self.audioProgress = (CGFloat)0.0;
   
        dispatch_async(dispatch_get_main_queue(), ^{
            [[NSNotificationCenter defaultCenter]  postNotificationName:kSynopsisOperationStateUpdate object:self.operationState];
        });
        
        self.initted = YES;
        
            NSActivityOptions options = NSActivityUserInitiated;
        self.processInfoActivity = [[NSProcessInfo processInfo] beginActivityWithOptions:options reason:@"Synopsis Analysis Session Started"];
    }

    return self;
}

- (void) dealloc
{
    NSLog(@"Dealloc Operation %@", [self className]);

    [[LogController sharedLogController] appendVerboseLog:[NSString stringWithFormat:@"Dealloc NSOperation %p", self, nil]];
}

- (void) start
{
    [[LogController sharedLogController] appendVerboseLog:[NSString stringWithFormat:@"Start Main NSOperation %p", self, nil]];

    self.operationState.operationState = OperationStateRunning;

    self.startDate = [NSDate date];
    self.elapsedTime = 0;
    self.remainingTime = DBL_MIN;

    [super start];
}

- (void) main
{
    
    @synchronized(self)
    {
        if(self.completionBlock)
        {
            self.operationState.operationState = OperationStateSuccess;
            
            [self setVideoProgress:1.0];
            [self setAudioProgress:1.0];
            [self notifyProgress];
            
            self.completionBlock();
            
            // Clear so we dont run twice, fucko
            self.completionBlock = nil;
        }
    }
    
    [[LogController sharedLogController] appendVerboseLog:[NSString stringWithFormat:@"Finish Main NSOperation %p", self, nil]];
    
    [[NSProcessInfo processInfo] endActivity:self.processInfoActivity];
}

- (void) cancel
{
    self.operationState.operationState = OperationStateCancelled;
    [self notifyProgress];

    [super cancel];
}

- (CGFloat) progress
{
    @synchronized(self)
    {
        self.operationState.operationProgress = (self.videoProgress * 0.9) + (self.audioProgress * 0.1);
        return self.operationState.operationProgress;
    }
}

- (CGFloat) videoProgress
{
    @synchronized(self)
    {
        return _videoProgress;
    }
}

- (void) setVideoProgress:(CGFloat)progress
{
    @synchronized(self)
    {
        _videoProgress = progress;
    }
    
    [self calculateEta];
        
}

- (CGFloat) audioProgress
{
    @synchronized(self)
    {
        return _audioProgress;
    }
}

- (void) setAudioProgress:(CGFloat)progress
{
    @synchronized(self)
    {
        _audioProgress = progress;
    }
    
    [self calculateEta];
}

- (void) calculateEta
{
    self.operationState.elapsedTime = [[NSDate date] timeIntervalSinceReferenceDate] - [self.startDate timeIntervalSinceReferenceDate];
    double itemsPerSecond = self.progress / self.operationState.elapsedTime;
    double secondsRemaining = (1.0 - self.progress) / itemsPerSecond;
    
    self.operationState.remainingTime = secondsRemaining;
    
    // send notification of updated progress and ETA
    if(self.initted)
    {
        [self notifyProgress];
    }
}

- (void) notifyProgress
{
    dispatch_async(dispatch_get_main_queue(), ^(){

        [[NSNotificationCenter defaultCenter] postNotificationName:kSynopsisOperationStateUpdate object:self.operationState];

    });
}

@end
