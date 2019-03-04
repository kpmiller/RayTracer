//
//  Document.m
//  RayTracer
//
//  Created by Kent Miller on 3/3/19.
//  Copyright © 2019 Kent Miller. All rights reserved.
//

#import "Document.h"
#import "RayTracer.hpp"

@interface Document ()

@property RTimage *rt;
@property NSTimer *timer;
@end

@implementation Document

- (instancetype)init {
    self = [super init];
    if (self) {
        // Add your subclass-specific initialization here.
    }
    self.rt = CreateRTimage(200,100);
    return self;
}

+ (BOOL)autosavesInPlace {
    return YES;
}

-(void) windowControllerDidLoadNib:(NSWindowController *)windowController
{
    self.timer = [NSTimer timerWithTimeInterval:0.2f target:self selector:@selector(updateImage) userInfo:nil repeats:YES];
    
    [[NSRunLoop currentRunLoop] addTimer:self.timer forMode:NSDefaultRunLoopMode];
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        Trace(self.rt);
    });
}
                  
- (NSString *)windowNibName {
    // Override returning the nib file name of the document
    // If you need to use a subclass of NSWindowController or if your document supports multiple NSWindowControllers, you should remove this method and override -makeWindowControllers instead.
    return @"Document";
}


- (NSData *)dataOfType:(NSString *)typeName error:(NSError **)outError {
    // Insert code here to write your document to data of the specified type. If outError != NULL, ensure that you create and set an appropriate error if you return nil.
    // Alternatively, you could remove this method and override -fileWrapperOfType:error:, -writeToURL:ofType:error:, or -writeToURL:ofType:forSaveOperation:originalContentsURL:error: instead.
    [NSException raise:@"UnimplementedMethod" format:@"%@ is unimplemented", NSStringFromSelector(_cmd)];
    return nil;
}


- (BOOL)readFromData:(NSData *)data ofType:(NSString *)typeName error:(NSError **)outError {
    // Insert code here to read your document from the given data of the specified type. If outError != NULL, ensure that you create and set an appropriate error if you return NO.
    // Alternatively, you could remove this method and override -readFromFileWrapper:ofType:error: or -readFromURL:ofType:error: instead.
    // If you do, you should also override -isEntireFileLoaded to return NO if the contents are lazily loaded.
    [NSException raise:@"UnimplementedMethod" format:@"%@ is unimplemented", NSStringFromSelector(_cmd)];
    return YES;
}


-(void) updateImage
{
    if ((self.rt == NULL) || (self.rt->data == NULL))
    {
        self.image.image = nil;
        return;
    }
    if (self.rt->changed == 0)
        return;
    
    self.rt->changed = 0;
    
    int w = self.rt->width;
    int h = self.rt->height;

    uint8_t *pixels = (uint8_t*) malloc(w*h*4);
    uint8_t *p = pixels;
    float *f = (float*) self.rt->data;
    for (int II = 0; II < w*h; II++)
    {
        p[0] = (uint8_t) (f[0] * 255.0);
        p[1] = (uint8_t) (f[1] * 255.0);
        p[2] = (uint8_t) (f[2] * 255.0);
        p[3] = 0xff;
        p+=4;
        f+=3;
    }

    //convert from RTImage (float rgb packed) to UIImage
    NSBitmapImageRep *bir = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:&pixels
            pixelsWide:self.rt->width
            pixelsHigh:self.rt->height
            bitsPerSample:8
            samplesPerPixel:4
            hasAlpha:YES
            isPlanar:NO
            colorSpaceName:NSDeviceRGBColorSpace
            bytesPerRow:self.rt->width*4
            bitsPerPixel:32];

    free(pixels);

    if (!bir)
    {
        NSLog(@"couldn't make NSBitmapImageRep");
        return;
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{
        CGImageSourceRef isr = CGImageSourceCreateWithData((CFDataRef)[bir TIFFRepresentation], NULL);
        CGImageRef image     = CGImageSourceCreateImageAtIndex(isr, 0, NULL);
        [self.image setImage:[[NSImage alloc] initWithCGImage:image size:NSMakeSize(w, h)]];
    });
}

@end
