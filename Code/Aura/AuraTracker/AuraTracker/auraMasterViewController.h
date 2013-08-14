//
//  auraMasterViewController.h
//  AuraTracker
//
//  Created by Cesar Torres on 7/29/13.
//  Copyright (c) 2013 Cesar Torres. All rights reserved.
//

#import <UIKit/UIKit.h>

@class auraDetailViewController;

@interface auraMasterViewController : UITableViewController

@property (strong, nonatomic) auraDetailViewController *detailViewController;

@end
