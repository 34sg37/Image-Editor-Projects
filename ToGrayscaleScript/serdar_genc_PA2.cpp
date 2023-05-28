#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class ImageEditor{
    public:
        void openImage(const string& name){ //name = file name
            ifstream read(name); //open read file
            if(!read){ //if no file
                cerr<<"Failed to open file "<<name<<endl;
                exit(5);
            }
            read >> format >> width >> height >> max_color; //format: image format(p3), width:columns, height: rows, max_color: max rgb values
            if (format != "P3"){ //we work with p3 only so if not p3 give error 
                cerr<<"File is not in ASCII PPM format."<<endl;
                exit(3);
            }
            if (!height || !width){ //if height or width is null give error
                cerr<<"The file is not valid."<<endl;
                exit(4);
            }
            //initialize and fill the vector with read values from the file, each cell represents a pixel and each sell contains rgb struct which contains r,g,b integers
            vector<vector<RGB>> vec(height);
            
            for (int i=0; i<height; i++){
                vec[i] =vector<RGB>(width);
                for (int j=0; j<width;j++){
                    read >> vec[i][j].r;
                    read >> vec[i][j].g;
                    read >> vec[i][j].b;
                }
            }
            read.close();
            pixels =vec;
        }
        void saveImage(const string& name){//name=file name
            ofstream write(name);//open write file
            if (!write){ //if file cant be created
                cerr<<"File could not be created"<<endl;
                exit(5); 
            }
            write << format <<endl << height << " "<< width << endl << max_color <<endl;//write format heighy width and max_color values first
            for (int i=0; i<height; i++){ //write the rgb values for each pixel
                for (int j=0; j<width;j++){
                    write << pixels[i][j].r <<" ";
                    write << pixels[i][j].g <<" ";
                    write << pixels[i][j].b <<" ";
                }
                write<<endl;
            }
            write.close();
        }
        void convertToGrayscale(float c_r, float c_g, float c_b){//c_r: red coefficient, c_g: green coefficient, c_b: blue coefficient
            for(int i=0; i<height; i++){ //for each pixel set r g b values to their grayscale value
                for(int j=0; j<width;j++){
                    int Y =  (c_r * pixels[i][j].r) + (c_g * pixels[i][j].g) + (c_b * pixels[i][j].b); //formula to find grayscale value
                    if (Y>255) Y=255; //if bigger than 255 set to 255 because 255 is max
                    
                    pixels[i][j].r = Y;
                    pixels[i][j].g = Y;
                    pixels[i][j].b = Y;
                }
            }
        }
    private:
        struct RGB{int r,g,b;};
        vector<vector<RGB>> pixels;
        int width,height,max_color;
        string format;
};

int main(){
    ImageEditor editor;
    char in1='1'; //input1
    int photoAmount=0;
    do{ //loop until 0 or invalid input entered
        char in2='1'; //input2
        bool valid=false; //validity of rgb coefficients
        string name; //name of file
        cout<<"MAIN MENU"<<endl<<"0 - Exit"<<endl<<"1 - Open An Image(D)"<<endl<<"2 - Save Image Data(D)"<<endl<<"3 - Scripts(D)"<<endl;
        cin>>in1;
        switch(in1){
            case '0': //exit
                return 0;
            case '1'://open image menu
                do{//loop until 0 is entered
                    cout<<"OPEN AN IMAGE MENU"<<endl<<"0 - UP"<<endl<<"1 - Enter The Name Of The Image File"<<endl;
                    cin>>in2;
                    if (in2=='1'){ //enter file name
                        cin>>name;
                        editor.openImage(name); 
                        photoAmount++;//photo added so increase photoamount
                    }
                    else if(in2!='0'){ //if invalid input give error and quit
                        cerr<<"Invalid Input"<<endl;
                        return 1;
                    }
                }while(in2!='0');
                break;
            case '2'://save image menu
                if(photoAmount==0){ //if there is no photo give error and quit
                    cerr<<"There is no photo to edit"<<endl;
                    return 2;
                }
                while(in2!='0'){// loop until 0 is entered
                    cout<<"SAVE IMAGE DATA MENU"<<endl<<"0 - UP"<<endl<<"1 - Enter A File Name"<<endl;
                    cin>>in2;
                    if(in2=='1'){ //enter file name
                        cin>>name;
                        editor.saveImage(name);
                    }
                    else if(in2!='0'){ //if invalid input give error and quit
                        cerr<<"Invalid Input"<<endl;
                        return 1;
                    }
                }
                break;
            case '3'://scripts menu
                if(photoAmount==0){ //if there is no photo give error and quit
                    cerr<<"There is no photo to edit"<<endl;
                    return 2;
                }
                do{//loop until 0 is entered
                    char in3='1';//input 3
                    cout<<"SCRIPTS MENU"<<endl<<"0 - UP"<<endl<<"1 - Convert To Grayscale(D)"<<endl;
                    cin>>in2;
                    if(in2=='1'){//grayscale menu
                        do{//loop until 0 is entered
                            cout<<"CONVERT TO GRAYSCALE MENU"<<endl<<"0 - UP"<<endl<<"1 - Enter Coefficients For RED GREEN And BLUE Channels."<<endl;
                            cin>>in3;
                            if(in3=='1'){
                                while (valid==false){//loop until valid input is entered
                                    float r,g,b; //rgb coefficients
                                    cin>>r>>g>>b;
                                    if (r>=0 && r<1 && g>=0 && g<1 && b>=0 && b<1)//check if valid
                                    {
                                        valid=true;//set to true if valid
                                        editor.convertToGrayscale(r,g,b);
                                    }
                                    else cout<<"You entered invalid input, please enter the inputs again."<<endl;
                                }
                            }
                            else if(in3!='0'){//if invalid input give error and quit
                                cerr<<"Invalid Input"<<endl;
                                return 1;
                            }
                        }while(in3!='0');
                    }
                    else if(in2!='0'){//if invalid input give error and quit
                        cerr<<"Invalid Input"<<endl;
                        return 1;
                    }
                }while(in2!='0');
                break;
            default: //if invalid input give error and quit
                cerr<<"Invalid Input"<<endl;
                return 1;
        }
    }while(in1!=0);
}