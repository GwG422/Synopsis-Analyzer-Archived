//
//  MotionModule.h
//  Synopsis
//
//  Created by vade on 11/10/16.
//  Copyright © 2016 metavisual. All rights reserved.
//

#import "Module.h"

@interface MotionModule : Module

- (NSDictionary*) analyzedMetadataForFrame:(matType)frame lastFrame:(matType)lastFrame;

@end
