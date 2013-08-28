//
//  auraDetailViewController.h
//  AuraTracker
//
//  Created by Cesar Torres on 7/29/13.
//  Copyright (c) 2013 Cesar Torres. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface auraDetailViewController : UIViewController <UISplitViewControllerDelegate>

@property (strong, nonatomic) id detailItem;

@property (weak, nonatomic) IBOutlet UILabel *detailDescriptionLabel;
@end
