//
//  MetadataWriterTranscodeOperation.h
//  MetadataTranscoderTestHarness
//
//  Created by vade on 4/4/15.
//  Copyright (c) 2015 Synopsis. All rights reserved.
//

#import "BaseTranscodeOperation.h"

@interface MetadataWriterTranscodeOperation : BaseTranscodeOperation

- (id) initWithUUID:(NSUUID*)uuid sourceURL:(NSURL*)sourceURL destinationURL:(NSURL*)destinationURL metadataOptions:(NSDictionary*)metadataOptions NS_DESIGNATED_INITIALIZER;
- (instancetype) init NS_UNAVAILABLE;

@end
