/**
 * This program "Recognize silhouettes" counts the number of people in the image.
 * To use the program you need to give black and white image
 * with silhouettes, and it will display the approximate result.
 * For correct operation of the program does not recommend
 * use of images in PNG format with a transparent background.
 *
 * For a convenient test program already has an image with names:
 * 1.jpg | 3.jpg | 4in1.jpg | 7.jpg | 8.jpg | 8-2.jpg | 86.jpg | mnogo.jpg
 *
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
GBufferedImage* drawObjectInLiline(VectorSHPP <VectorSHPP<Point>>setObjects, int maxWidth, int maxHeight);
void setSizeWindow(VectorSHPP <VectorSHPP<Point>> setObjects, int maxHeight, int maxWidth, GBufferedImage* imageInLine);

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
    while(true){
        string name;
        cout << "Enter the name of the image file : ";
        cin >> name;
        cout << "Processing..." << endl;

        // displays the initial image
        GBufferedImage* image = new GBufferedImage();
        image->load(name);

        // It counts the number of objects and closes the screen with the original image
        VectorSHPP<VectorSHPP<Point>> setObjects = findObjects(image);
        cout << "find " << setObjects.size() << " objects" << endl;

        // It displays all the objects in a row
        GBufferedImage* imageInLine;
        imageInLine = drawObjectInLiline(setObjects, image->getWidth(), image->getHeight());
        delete image;
        // calculates the pixels of Y coordinates, draws the schedule and outputs the result
        //  GWindow resWindow;
        GBufferedImage *newImage = new GBufferedImage(imageInLine->getWidth(), HEIGHT_IMAGE_GRAPH, 0xffffff);
        //  resWindow.setCanvasSize(imageInLine->getWidth(), HEIGHT_IMAGE_GRAPH);
        //  resWindow.add(newImage);
        findPeople(imageInLine, newImage);
        delete imageInLine;
        delete newImage;
    }
    return 0;

}

/**
 * Method: findObjects
 * Usage: VectorSHPP <VectorSHPP<Point>> objects =  findObjects(GBufferedImage* image)
 * ___________________________________________________________________________________
 *
 * Processes all pixels of the image, if it finds a point of object, that will bring method
 * which collect together entire object, and return it to the point of vector
 * points. At the end of method will be compiled vector of all objects.
 *
 * @param image - image stream
 * @return - two dimensional vector of the points of the objects
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
 * Method: collectObject
 * Usage: VectorSHPP<Point> object = collectObject(int x, int y, GBufferedImage* image)
 * ____________________________________________________________________________________
 *
 * Takes the point at which an object is found, and looking for all the neighboring
 * points to detect the entire object.
 *
 * @param x - point coordinate on the X axis
 * @param y - point coordinate on the Y axis
 * @param image - image stream
 * @return - Vector points of one object
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
 * Method: drawObjectInLiline
 * Usage: GBufferedImage* objectsInLine = drawObjectInLiline(VectorSHPP <VectorSHPP<Point>>setObjects,
 *                                               int maxWidth, int maxHeight)
 * _____________________________________________________________________________________________________
 *
 * This method specifies size of new picture, the height same the highest object multiplied by 1.3
 * (this is the number of attached to the free space at the top of image), and the width equal to the
 * width of all objects. After that draw objects on a new image in a line.
 *
 * @param setObjects - two dimensional vector of the points of the objects
 * @param maxWidth - width of the original image, it need for calculate the min and max size of the object
 * @param maxHeight - height of the original image, it need for calculate the min and max size of the object
 * @return - image stream in which all objects are located in one line
 */
GBufferedImage* drawObjectInLiline(VectorSHPP <VectorSHPP<Point>>setObjects, int maxWidth, int maxHeight){
    GBufferedImage* imageInLine = new GBufferedImage();
    int heightTmp;
    int startXPos = 0;

    // sets the size of the new window and images
    setSizeWindow(setObjects, maxHeight, maxWidth, imageInLine);
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
    return imageInLine;
}

/**
 * Method: setSizeWindow
 * Usage: setSizeWindow(VectorSHPP <VectorSHPP<Point>> setObjects, int maxHeight, int maxWidth,
 *                                          GBufferedImage* imageInLine)
 * __________________________________________________________________________________________________
 *
 * Set size for new image, building on size of the objects
 *
 * @param setObjects - two dimensional vector of the points of the objects
 * @param maxHeight - height of the original image, it need for calculate the min and max size of the object
 * @param maxWidth - width of the original image, it need for calculate the min and max size of the object
 * @param imageInLine - image where will be placed all objects
 */
void setSizeWindow(VectorSHPP <VectorSHPP<Point>> setObjects, int maxHeight, int maxWidth, GBufferedImage* imageInLine){
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
}

/**
 * Method: findPeople
 * Using: findPeople(GBufferedImage *image, GBufferedImage *newImage)
 * __________________________________________________________________
 *
 * Method processes images with objects, calculates the "weight" of the axes Y,
 * after which visualizes it as a graph, and counts the number of people on graph.
 *
 * @param image - image stream, which shows all objects
 * @param newImage - image stream to visualize graphics
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
 * Method: sumPixelsInOneYCoordinate
 * Usage: float sumPixels = sumPixelsInOneYCoordinate(int x, GBufferedImage *image)
 * ________________________________________________________________________________
 *
 * Method calculates the number of each black pixels in axis Y, as well as all the
 * neighboring pixels. For each pixel calculated a certain number, the number becomes
 * smaller when the neighboring pixels being away, as well as the number becomes
 * smaller towards the bottom of the image.
 *
 * @param x - X coordinate on which needs to process all pixels on the Y axis
 * @param image - stream image
 * @return "weight" Y axis
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
 * Method: findMaxPixelsCol
 * Usage: float max = findMaxPixelsCol(VectorSHPP <float> &sumPixelsInWidth)
 * _________________________________________________________________________
 *
 * This method finds the greatest value of the number of vector
 *
 * @param sumPixelsInWidth - Vector with numbers
 * @return the maximum number
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
 * Method: stabiliationPixels
 * Using: VectorSHPP<int> stableNumber = stabiliationPixels(VectorSHPP <float> &sumPixelsInWidth, float maxNumber)
 * _______________________________________________________________________________________________________________
 *
 * This method stabilizes all numbers in a percentage range from 0 to 100
 *
 * @param sumPixelsInWidth - vector numbers type float
 * @param maxNumber - The maximum number of the input vector
 * @return - vector numbers type int
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
 * Method: flipArrayValues
 * Usage: VectorSHPP<int> flipValues = flipArrayValues(VectorSHPP<int> stabSum)
 * ___________________________________________________________________________
 *
 * Receives vector Int and makes negative values
 *
 * @param stabSum - vector values type int
 * @return - returns the input vector with negative values
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
 * Method: drawGraph
 * Usage: drawGraph(VectorSHPP<int> &stabSum, GBufferedImage *newImage)
 * ___________________________________________________________________
 *
 * Receives vector of int where each value - a visualization of the
 * "weight" of Y coordinate and draws these values on the graph.
 *
 * @param stabSum - vector of numbers from 0 to 100
 * @param newImage - stream image
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
 * Method: goThroughLine
 * Usage: int colPeople = goThroughLine(VectorSHPP<int> &line)
 * ___________________________________________________________
 *
 * Method considers delta from the chart. If in the last five
 * pixels has been the growth of delta for more than 10 pixels,
 * and shortly thereafter follow descent of delta for more than
 * 10 pixels, then method counts one person.
 *
 * @param line - vector of numbers from 0 to 100
 * @return - number of people silhouettes
 */
int goThroughLine(VectorSHPP<int> &line){
    int result = 0;
    int dy = 0;
    bool trigUp = false;

    for(int i = 0; i < line.size(); i++){
        if (i >= 5) dy = line[i - 5] - line[i];

        if (dy >= 10) trigUp = true;

        if (dy <= -10 && trigUp){
            result++;
            trigUp = false;
        }
    }
    return result;
}
