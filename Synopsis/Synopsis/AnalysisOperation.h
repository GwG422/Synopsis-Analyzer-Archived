//
//  AnalysisOperation.h
//  Synopsis Analyzer
//
//  Created by vade on 6/20/19.
//  Copyright © 2019 metavisual. All rights reserved.
//

#import "BaseTranscodeOperation.h"

NS_ASSUME_NONNULL_BEGIN

@interface AnalysisOperation : BaseTranscodeOperation

- (instancetype) initWithOperationState:(OperationStateWrapper*)operationState sourceURL:(NSURL*)sourceURL destinationURL:(NSURL*)destinationURL transcodeOptions:(NSDictionary*)transcodeOptions NS_DESIGNATED_INITIALIZER;


@end

NS_ASSUME_NONNULL_END
