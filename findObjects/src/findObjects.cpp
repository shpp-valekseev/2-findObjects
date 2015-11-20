/**
 * This program "Recognize silhouettes" counts the number of people in the image.
 * To use the program you need to give black and white image
 * with silhouettes, and it will display the approximate result.
 * For correct operation of the program does not recommend
 * the use of images in PNG format with a transparent background.
 */

#include <iostream>
#include <string>
#include "gbufferedimage.h"
#include "console.h"
#include "point.h"
#include "queueshpp.h"
#include "vectorshpp.h"
#include "gobjects.h"
using namespace std;

// contains a value in pixels, the minimum size of an object
const int MIN_OBJECT_SIZE = 50;

// height of the image which will draw graphics
const int HEIGHT_IMAGE_GRAPH = 200;

// the distance between objects when they will draw in a line
const int SIZE_BETWEEN_OBJECTS = 5;

// function prototypes
VectorSHPP <VectorSHPP<Point>> findObjects(GBufferedImage* image);
VectorSHPP <Point> collectObject (int x, int y, GBufferedImage* image);
GBufferedImage* drawObjectInLiline(VectorSHPP <VectorSHPP<Point>>setObjects, GWindow &windowForLineImage, int maxWidth, int maxHeight);
void setSizeWindow(VectorSHPP <VectorSHPP<Point>> setObjects, int maxHeight, int maxWidth, GBufferedImage* imageInLine, GWindow &windowForLineImage);

void findPeople(GBufferedImage *image, GBufferedImage *newImage);
float sumPixelsInOneYCoordinate(int x, GBufferedImage *image);
float findMaxPixelsCol(VectorSHPP <float> &sumPixelsInWidth);
VectorSHPP<int> stabiliationPixels(VectorSHPP <float> &sumPixelsInWidth, float maxNumber);
VectorSHPP<int> flipArrayValues(VectorSHPP<int> stabSum);
void drawGraph(VectorSHPP<int> &stabSum, GBufferedImage *newImage);
int goThroughLine(VectorSHPP<int> &line);


/**
 * The main method of program that asks the user for the name of
 * the file, reads the image and calls a method to detect people
 */
int main() {
    cout << "Welcome to 'Recognize silhouettes' program." << endl;
    string name;
    cout << "Enter the name of the image file : ";
    cin >> name;
    cout << "Processing..." << endl;

    // displays the initial image
    GWindow window;
    GBufferedImage* image = new GBufferedImage();
    image->load(name);
    window.setCanvasSize(image->getWidth(),image->getHeight());
    window.add(image);

    // It counts the number of objects and closes the screen with the original image
    VectorSHPP<VectorSHPP<Point>> setObjects = findObjects(image);
    cout << "find " << setObjects.size() << " objects" << endl;
    window.close();

    // It displays all the objects in a row
    GWindow windowForLineImage;
    GBufferedImage* imageInLine;
    imageInLine = drawObjectInLiline(setObjects, windowForLineImage, image->getWidth(), image->getHeight());
    delete image;
    // calculates the pixels of Y coordinates, draws the schedule and outputs the result
    GWindow resWindow;
    GBufferedImage *newImage = new GBufferedImage(imageInLine->getWidth(), HEIGHT_IMAGE_GRAPH, 0xffffff);
    resWindow.setCanvasSize(imageInLine->getWidth(), HEIGHT_IMAGE_GRAPH);
    resWindow.add(newImage);
    findPeople(imageInLine, newImage);
    delete imageInLine;
    delete newImage;
    return 0;

}

/**
 * The method takes the picture.Processes all the pixels of the image,
 * if it finds a object point is method that will bring together the entire object,
 * and return it to the point of vector points. At the end of the method will be
 * compiled vector of all objects. The method returns a vector objects.
 */
VectorSHPP <VectorSHPP<Point>>  findObjects(GBufferedImage* image){
    VectorSHPP <VectorSHPP<Point>> setObjects;

    for (int y = 0; y < image->getHeight(); y++){
        for (int x = 0; x < image->getWidth(); x++){
            if (image->getRGB(x,y) <= 0xefefef){
                image->setRGB(x,y,0xffffff);
                VectorSHPP <Point> tmp = collectObject(x, y, image);
                // if the object is smaller than the minimum size, it will not be counted
                if(tmp.size() > MIN_OBJECT_SIZE){
                    setObjects.add(tmp);
                }
            }
        }
    }
    return setObjects;
}

/**
 * Takes the point at which an object is found, and looking for
 * all the neighboring points to detect the entire object.
 * Returns points of a vector object
 */
VectorSHPP <Point> collectObject (int x, int y, GBufferedImage* image){
    VectorSHPP <Point> result;
    QueueSHPP <Point> points;
    Point point(x,y);
    points.enqueue(point);

    while(!points.isEmpty()){
        Point newPoint = points.dequeue();
        result.add(newPoint);
        for(int x = newPoint.getX()-1; x <= newPoint.getX()+1; x++){
            if (x < image->getWidth()-1 && x > 1){
                for(int y = newPoint.getY()-1; y <= newPoint.getY()+1; y++){
                    if (y < image->getHeight()-1 && y > 1){
                        if (image->getRGB(x,y) <= 0xefefef){
                            Point tmpPoint(x,y);
                            points.enqueue(tmpPoint);
                            image->setRGB(tmpPoint.getX(),tmpPoint.getY(),0xffffff);
                        }
                    }
                }
            }
        }
    }

    return result;
}

/**
 * reads from the array of each point of the object,
 * and returns the image where the objects are located in a row
 */
GBufferedImage* drawObjectInLiline(VectorSHPP <VectorSHPP<Point>>setObjects, GWindow &windowForLineImage, int maxWidth, int maxHeight){
    GBufferedImage* imageInLine = new GBufferedImage();
    int heightTmp;
    int startXPos = 0;

    // sets the size of the new window and images
    setSizeWindow(setObjects, maxHeight, maxWidth, imageInLine, windowForLineImage);
    int heightWindow = imageInLine->getHeight();

    // takes every object from the array
    for(int j = 0; j < setObjects.size(); j++){
        VectorSHPP<Point> object = setObjects.get(j);
        int minX = maxWidth;
        int maxX = 0;
        int minY = maxHeight;
        int maxY = 0;

        // calculates the minimum and maximum coordinate values of X and Y, for each object
        for (int i = 0; i < object.size(); i++){
            Point pointTmp = object[i];
            int pointX = pointTmp.getX();
            int pointY = pointTmp.getY();

            if(pointX < minX) minX = pointX;
            else if (pointX > maxX) maxX = pointX;

            if(pointY < minY) minY = pointY;
            else if (pointY > maxY) maxY = pointY;
        }

        // the height of each object
        heightTmp =  maxY - minY;

        // draws all objects in one line
        for (int i = 0; i < object.size(); i++){
            Point point = object.get(i);

            int tmpX = (point.getX() - minX) + startXPos;
            int tmpY = (heightWindow - heightTmp) + (point.getY() - minY);
            imageInLine->setRGB(tmpX, tmpY - 1, 0x000000);
        }
        startXPos = startXPos + (maxX - minX) + SIZE_BETWEEN_OBJECTS; // change start position of the X coordinate for each object

    }
    windowForLineImage.close();
    return imageInLine;
}

/**
 * Takes an array objects, new image and new window. Sets the window, and image size.
 */
void setSizeWindow(VectorSHPP <VectorSHPP<Point>> setObjects, int maxHeight, int maxWidth, GBufferedImage* imageInLine, GWindow &windowForLineImage){
    int height;
    int width;
    int maxHeightObject = 0;
    int widthWindow = 0;

    // takes every object from the array
    for(int i = 0; i < setObjects.size(); i++){
        VectorSHPP<Point> object = setObjects[i];
        int minY = maxHeight;
        int maxY = 0;
        int minX = maxWidth;
        int maxX = 0;

        // check each point of the object, and calculates
        // the width of all objects, and the height of the highest object
        for(int j = 0; j < object.size(); j++){
            Point tmpPoint = object[j];
            int pointY = tmpPoint.getY();
            int pointX = tmpPoint.getX();

            if(pointY < minY) minY = pointY;
            else if (pointY > maxY) maxY = pointY;

            if(pointX < minX) minX = pointX;
            else if (pointX > maxX) maxX = pointX;
        }
        width = maxX - minX;
        height =  maxY - minY;
        widthWindow += (width + SIZE_BETWEEN_OBJECTS);
        if(height > maxHeightObject){
            maxHeightObject = height;
        }
    }

    // sets the size of window and image
    imageInLine->resize(widthWindow, maxHeightObject * 1.3);
    imageInLine->fill(0xffffff);
    windowForLineImage.setCanvasSize(imageInLine->getWidth(), imageInLine->getHeight());
    windowForLineImage.add(imageInLine);
}

/**
 * The method takes two windows, one with the original image,
 * and the second to visualize the schedule
 */
void findPeople(GBufferedImage *image, GBufferedImage *newImage){
    VectorSHPP <float> sumPixelsInWidth;
    for (int x = 1; x < image->getWidth()-1; x++){
        sumPixelsInWidth.add(sumPixelsInOneYCoordinate(x, image));
    }
    float maxNumber = findMaxPixelsCol(sumPixelsInWidth);
    VectorSHPP<int> stabSum = stabiliationPixels(sumPixelsInWidth, maxNumber);
    stabSum = flipArrayValues(stabSum);
    drawGraph(stabSum, newImage);
    int res = goThroughLine(stabSum);
    if (res > 1){
        cout << "On this image, we found approx " << res << " peoples" << endl;
    } else {
        cout << "On this image, we found approx " << res << " people" << endl;
    }
}

/**
 * The method goes through each pixel of the Y-axis,
 * and counts the number of black pixels surrounding
 * "the cost" of pixels become smaller closer to the legs
 */
float sumPixelsInOneYCoordinate(int x, GBufferedImage *image){
    float col = 0;

    float yv = 10; // reduces the "cost" of each pixel is closer to the legs
    for (int y = 1; y < image->getHeight()-1; y++){
        yv = yv/1.01;
        if (image->getRGB(x,y) < 0xefefef){
            int tmpX = x;
            float i = yv;
            // considers all the black pixels to the left
            if(tmpX > 1){
                while(image->getRGB(tmpX-1,y) < 0xefefef && tmpX-1 > 1){
                    col = col + i;
                    i = i/1.1;
                    tmpX--;
                }
            }
            tmpX = x;
            i = yv;
            if(tmpX < image->getWidth()-1){
                // considers all the black pixels to the right
                while(image->getRGB(tmpX+1,y) < 0xefefef && tmpX+1 < image->getWidth()-1){
                    col = col + i;
                    i = i/1.1;
                    tmpX++;
                }
            }
        }
    }
    return col;
}

/**
 * Accepts vector float data type, and returns the largest value of its
 */
float findMaxPixelsCol(VectorSHPP <float> &sumPixelsInWidth){
    int maxNumber = 0;
    for(int i = 0; i < sumPixelsInWidth.size(); i++){
        int tmp = sumPixelsInWidth[i];
        if(tmp > maxNumber) maxNumber = tmp;
    }
    return maxNumber;
}

/**
 * Receives data vector type float and returns the same
 * vector with stable values array percentage range (0 to 1)
 */
VectorSHPP<int> stabiliationPixels(VectorSHPP <float> &sumPixelsInWidth, float maxNumber){
    VectorSHPP<int> res;
    for(int i = 0; i < sumPixelsInWidth.size(); i++){
        int number = sumPixelsInWidth.get(i);
        if(number == 0){
            res.add(0);
        } else {
            float tmp = (number/maxNumber);
            res.add((int)(tmp*100));
        }
    }
    return res;
}

/**
 * Receives vector int, and returns the same vector with inverted values
 */
VectorSHPP<int> flipArrayValues(VectorSHPP<int> stabSum){
    VectorSHPP<int> res;
    for(int i = 0; i < stabSum.size(); i++){
        int y = stabSum[i];
        res.add(y*(-1));
    }
    return res;
}

/**
 * Receives vector int, which are processed by the
 * coordinate values Y, and displays them as a graph.
 */
void drawGraph(VectorSHPP<int> &stabSum, GBufferedImage *newImage){
    int x = 1;
    for(int i = 0; i < stabSum.size(); i++){
        int y = stabSum[i];
        newImage->setRGB(x, y + (newImage->getHeight()-1), 0x000000);
        x++;
    }
}

/**
 * Processes "schedule", and decides whether there is
 * a human figure. Returns the number of people in the image.
 */
int goThroughLine(VectorSHPP<int> &line){
    int result = 0;
    int dy = 0;
    bool trigUpH = false;
    bool trigUpL = false;

    for(int i = 0; i < line.size(); i++){
        if (i >= 5) dy = line[i - 5] - line[i];
        if (dy >= 10) trigUpH = true;
        if (dy <= 50 && dy > 0 && trigUpH) trigUpL = true;

        if (dy <= -10 && trigUpH && trigUpL){
            result++;
            trigUpH = false;
            trigUpL = false;
        }
    }
    return result;
}
