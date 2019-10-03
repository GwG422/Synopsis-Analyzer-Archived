//
//  BaseTranscodeOperation.h
//  MetadataTranscoderTestHarness
//
//  Created by vade on 4/4/15.
//  Copyright (c) 2015 Synopsis. All rights reserved.
//

#import <Synopsis/Synopsis.h>
#import <Foundation/Foundation.h>
#import "LogController.h"
#import "OperationStateWrapper.h"


#pragma mark - Pass 1 Settings:

// Key whose value is a dictionary appropriate for use with AVAssetWriterInput output settings. See AVVideoSettings.h
// If this key is [NSNull null] it implies passthrough video encoding - sample buffers will not be re-encoded;
// Required
extern NSString* const kSynopsisTranscodeVideoSettingsKey;

// Key whose value is a dictionary appropriate for use with AVAssetWriterInput output settings. See AVAudioSettings.h
// If this key is [NSNull null] it implies passthrough video encoding - sample buffers will not be re-encoded;
// Required
extern NSString* const kSynopsisTranscodeAudioSettingsKey;

// Key whose value is an NSDictionary with the following keys and values (see below)
// Required
extern NSString * const kSynopsisAnalysisSettingsKey;

// Key whose value is an NSNumber wrapping a SynopsisAnalysisQualityHint to use for the analysis session.
extern NSString * const kSynopsisAnalysisSettingsQualityHintKey;

// Key whose value is an NSNumber wrapping a boolean to enable threaded / concurrent analysis for modules and plugins.
extern NSString * const kSynopsisAnalysisSettingsEnableConcurrencyKey;

// Key whose value is an NSArray of NSStrings which are classnames of enabled pluggins used for the analysis session
extern NSString * const kSynopsisAnalysisSettingsEnabledPluginsKey;

// Key whose value is an NSDictionary of key value pairs of Encoder class names and an array of NSStrings modules enabled.
extern NSString * const kSynopsisAnalysisSettingsEnabledPluginModulesKey;

#pragma mark - Pass 2 Settings:

// Key whose value is an NSArray of AVMetadataItem (format TBD, we need to include time, maybe CMTimeRangeValues ?)
// The existence of this key implies we will write a metadata track associated with our video track
// Optional
extern NSString* const kSynopsisAnalyzedVideoSampleBufferMetadataKey;

// TODO: Implement this and think about audio metadata.
// Key whose value is an NSArray of metadata dictionaries (format TBD, we need to include time, maybe CMTimeRangeValues ?)
// The existence of this key implies we will write a metadata track associated with our video track.
// Optional
extern NSString* const kSynopsisAnalyzedAudioSampleBufferMetadataKey;

// Key whose value is a AVMetadataItem containing aggregate overall metadata used to write a summary metadata entry.
// The existence of this key implies we will write a metadata track with no association
// Optional
extern NSString* const kSynopsisAnalyzedGlobalMetadataKey;

// Optional - export JSON ?
extern NSString * const kSynopsisAnalyzedMetadataExportOptionKey;

// Base Transcode Operation ensures that its completion block runs WITHIN MAIN
// So that you know that depended operations have the previous compltion block called.
// Otherwise, you have a race condition.
// And thats fucking stupid.


@interface BaseTranscodeOperation : NSOperation
@property (atomic, readonly, strong) NSURL* sourceURL;
@property (atomic, readonly, strong) NSURL* destinationURL;
@property (atomic, readonly, strong) OperationStateWrapper* operationState;

@property (atomic, readonly) CGFloat progress;

// internal use - exposed for subclasses
@property (atomic, readwrite) CGFloat videoProgress;
@property (atomic, readwrite) CGFloat audioProgress;

@property (readonly, assign) BOOL succeeded;

@property (readonly, strong) NSError* error;

//// Every progress update tick this block is fired - update your ui on the main queue here.
//@property (copy) void (^progressBlock)(CGFloat progress);

- (instancetype) initWithOperationState:(OperationStateWrapper*)operationState sourceURL:(NSURL*)sourceURL destinationURL:(NSURL*)destinationURL;
- (void) start NS_REQUIRES_SUPER;
- (void) main NS_REQUIRES_SUPER;


@end
