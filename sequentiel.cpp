#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <msclr\marshal_cppstd.h>
#include <ctime> // include this header
#pragma once

#using < mscorlib.dll>
#using < System.dll>
#using < System.Drawing.dll>
#using < System.Windows.Forms.dll>
using namespace std;
using namespace msclr::interop;

int *inputImage(int *w, int *h, System::String ^ imagePath) // put the size of image in w & h
{
    int *input;

    int OriginalImageWidth, OriginalImageHeight;

    //*********************************************************Read Image and save it to local arrayss*************************
    // Read Image and save it to local arrayss

    System::Drawing::Bitmap BM(imagePath);

    OriginalImageWidth = BM.Width;
    OriginalImageHeight = BM.Height;
    *w = BM.Width;
    *h = BM.Height;
    int *Red = new int[BM.Height * BM.Width];
    int *Green = new int[BM.Height * BM.Width];
    int *Blue = new int[BM.Height * BM.Width];
    input = new int[BM.Height * BM.Width];
    for (int i = 0; i < BM.Height; i++)
    {
        for (int j = 0; j < BM.Width; j++)
        {
            System::Drawing::Color c = BM.GetPixel(j, i);

            Red[i * BM.Width + j] = c.R;
            Blue[i * BM.Width + j] = c.B;
            Green[i * BM.Width + j] = c.G;

            input[i * BM.Width + j] = ((c.R + c.B + c.G) / 3); // gray scale value equals the average of RGB values
        }
    }
    return input;
}

void createImage(int *image, int width, int height, int index)
{
    System::Drawing::Bitmap MyNewImage(width, height);

    for (int i = 0; i < MyNewImage.Height; i++)
    {
        for (int j = 0; j < MyNewImage.Width; j++)
        {
            // i * OriginalImageWidth + j
            if (image[i * width + j] < 0)
            {
                image[i * width + j] = 0;
            }
            if (image[i * width + j] > 255)
            {
                image[i * width + j] = 255;
            }
            System::Drawing::Color c = System::Drawing::Color::FromArgb(image[i * MyNewImage.Width + j], image[i * MyNewImage.Width + j], image[i * MyNewImage.Width + j]);
            MyNewImage.SetPixel(j, i, c);
        }
    }
    MyNewImage.Save("..//Data//Output//outputRes" + index + ".png");
    cout << "result Image Saved " << index << endl;
}

int sum(int *imageData, int ImageHeight, int ImageWidth, int j)
{
    int sum = 0;
    int count = 0;
    int count2 = 0;
    for (; j < (ImageHeight * ImageWidth); j++)
    {
        sum += imageData[j];
        count++;
        if (count == 3)
        {
            count = 0;
            count2++;
            j = j + ImageWidth - 2;
        }
        if (count2 == 3)
        {
            count2 = 0;
            break;
        }
    }
    return (sum / 9);
}

int main()
{
    int ImageWidth = 4, ImageHeight = 4;
    int start_s, stop_s, TotalTime = 0;
    System::String ^ imagePath;
    std::string img;
    img = "..//Data//Input//test.png";
    imagePath = marshal_as<System::String ^>(img);
    int *imageData = inputImage(&ImageWidth, &ImageHeight, imagePath);
    start_s = clock();

    int *imageDataBorder = new int[(ImageHeight + 2) * (ImageWidth + 2)];

    // + border
    vector<int> imageDataBorderVEC;

    // add border to the first row
    /*for (int i = 0; i <  ImageWidth + 2; i++)
    {
        imageDataBorderVEC.push_back(0);
    }*/

    int col = 0;
    int invec = 0;

    for (int i = 0; i < (ImageWidth + 2) * ImageHeight; i++)
    {
        // Border to the first colum
        if (col == 0)
        {
            imageDataBorderVEC.push_back(0);
            col++;
            continue;
        }
        // Border to the last colum
        if (col == ImageWidth + 1)
        {
            imageDataBorderVEC.push_back(0);
            col++;
            col = 0;
            continue;
        }
        // fill matrix with real data
        else
        {
            imageDataBorderVEC.push_back(imageData[invec]);
            col++;
            invec++;
        }
    }

    // Border to the last row
    for (int i = 0; i < ImageWidth + 2; i++)
    {
        imageDataBorderVEC.push_back(0);
        imageDataBorderVEC.insert(imageDataBorderVEC.begin(), 0);
    }

    // get data from vector to pointer
    for (int i = 0; i < imageDataBorderVEC.size(); i++)
    {
        imageDataBorder[i] = imageDataBorderVEC[i];
    }

    cout << " data num    : " << imageDataBorderVEC.size() << endl;

    vector<int> newimg;
    int *data = new int[(ImageHeight) * (ImageWidth)];
    int var = ImageWidth * ImageHeight;
    cout << "the old size is  : " << var << endl;
    int i = 0;
    int temp = 0;
    int index = 0;

    while (true)
    {
        temp++;

        // get the end of matrix
        if (i == (ImageWidth * ImageHeight) - (2 * ImageWidth) - 2)
        {
            break;
        }

        // to move Block
        if (temp == ImageWidth - 1)
        {
            temp = 0;
            i += 2;
        }

        // call fun and make the algorithm
        data[index] = sum(imageDataBorder, ImageHeight + 2, ImageWidth + 2, i);

        newimg.push_back(sum(imageDataBorder, ImageHeight + 2, ImageWidth + 2, i));
        index++;
        i++;
    }
    cout << "the new size is " << newimg.size() << endl;
    cout << "the diff is  : " << var - newimg.size() << endl;
    createImage(data, ImageWidth, ImageHeight, 0);
    stop_s = clock();
    TotalTime += (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000;
    cout << "time: " << TotalTime << endl;
    free(imageData);
    system("pause");
    return 0;
}
