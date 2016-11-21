//
//  TensorFlowAnalyzer.h
//  Synopsis
//
//  Created by vade on 12/6/15.
//  Copyright (c) 2015 metavisual. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreMedia/CoreMedia.h>
#import "AnalyzerPluginProtocol.h"

@interface TensorFlowAnalyzer : NSObject <AnalyzerPluginProtocol>

@property (readonly) NSString* pluginName;
@property (readonly) NSString* pluginIdentifier;

@property (readonly) NSArray* pluginAuthors;
@property (readonly) NSString* pluginDescription;

@property (readonly) NSUInteger pluginAPIVersionMajor;
@property (readonly) NSUInteger pluginAPIVersionMinor;

@property (readonly) NSUInteger pluginVersionMajor;
@property (readonly) NSUInteger pluginVersionMinor;

@property (readonly) NSDictionary* pluginReturnedMetadataKeysAndDataTypes;

@property (readonly) NSString* pluginMediaType;

@property (readonly) NSArray* pluginModuleNames;

// Logging callbacks fo inclusion in the UI
@property (copy) LogBlock errorLog;
@property (copy) LogBlock successLog;
@property (copy) LogBlock warningLog;
@property (copy) LogBlock verboseLog;


@property (readonly) BOOL hasModules;

- (void) beginMetadataAnalysisSessionWithQuality:(SynopsisAnalysisQualityHint)qualityHint;

- (void) submitAndCacheCurrentVideoBuffer:(void*)baseAddress width:(size_t)width height:(size_t)height bytesPerRow:(size_t)bytesPerRow;

- (NSDictionary*) analyzeMetadataDictionaryForModuleIndex:(SynopsisModuleIndex)moduleIndex error:(NSError**)error;

- (NSDictionary*) finalizeMetadataAnalysisSessionWithError:(NSError**)error;

@end
