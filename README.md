# Leap Motion Particle System

This is a particle system driven by a leap motion. Moving your hand around pushes particles around on screen, and closing your hand into a fist makes the particles attract toward it.

In order to get this running, you'll need a leap motion sensor, the leap motion 2.3 sdk, and the ofxLeapMotion2 addon.

To get the ofxLeapMotion2 addon working properly, you need to add the following run script build phase:
```
cp -f ../../../addons/ofxLeapMotion2/libs/lib/osx/libLeap.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/libLeap.dylib"; install_name_tool -change ./libLeap.dylib @executable_path/libLeap.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/$PRODUCT_NAME";
```
