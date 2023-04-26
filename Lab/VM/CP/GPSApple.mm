#include <stdio.h>
#include <CoreLocation/CoreLocation.h>

@interface MyLocationDelegate : NSObject <CLLocationManagerDelegate>
@end

@implementation MyLocationDelegate

- (void)locationManager:(CLLocationManager *)manager didUpdateLocations:(NSArray *)locations {
    CLLocation *location = [locations lastObject];
    printf("Latitude: %f\n", location.coordinate.latitude);
    printf("Longitude: %f\n", location.coordinate.longitude);
}

- (void)locationManager:(CLLocationManager *)manager didFailWithError:(NSError *)error {
    printf("Failed to get location\n");
}

@end

int main() {
    CLLocationManager *locationManager = [[CLLocationManager alloc] init];
    MyLocationDelegate *delegate = [[MyLocationDelegate alloc] init];
    locationManager.delegate = delegate;
    locationManager.distanceFilter = kCLDistanceFilterNone; // Update location whenever it changes
    locationManager.desiredAccuracy = kCLLocationAccuracyBest;

    [locationManager startUpdatingLocation];

    CFRunLoopRun();

    return 0;
}
