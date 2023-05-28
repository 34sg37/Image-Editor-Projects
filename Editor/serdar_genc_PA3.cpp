#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

bool check_file(const string filename);

class ppmImage{
    public:
        ppmImage(){}
        ppmImage(const string& name) {openImage(name);} //calls openImage function to read file named "name" and create its object.
        // user-defined constructor to set height=h, width=w, max_color= m_c, pixels=p and format="P3"(as default)
        ppmImage(int h, int w, int m_c, vector<vector<vector<int>>> p) : height(h), width(w), max_color(m_c), pixels(p), format("P3"){} 
        void openImage(const string& name);
        void saveImage(const string& name)const;
        int pixelInfo(int h, int w, int c)const;
        ppmImage operator+(const ppmImage& other)const;
        ppmImage operator-(const ppmImage& other)const;
        friend ostream& operator<<(ostream& output, const ppmImage& cl);
        int& operator()(int h, int w, int c);
        const int& operator()(int h, int w, int c)const;
        //getters
        int getHeight()const {return height;}
        int getWidth()const {return width;}
        int getMaxColor()const {return max_color;}
        
    private:
        vector<vector<vector<int>>> pixels;
        int width,height,max_color;
        string format;
};

int read_ppm(const string source_ppm_file_name, ppmImage& destination_object);
int write_ppm(const string destination_ppm_file_name, const ppmImage& source_object);
int test_addition(const string filename_image1, const string filename_image2, const string filename_image3);
int test_subtraction(const string filename_image1, const string filename_image2, const string filename_image3);
int test_print(const string filename_image1);
int swap_channels(ppmImage& image_object_to_be_modified, int swap_choice);
ppmImage single_color(const ppmImage& source, int color_choice);

int main(int argc, char** argv){
    string ppm_file_name1,ppm_file_name2,ppm_file_name3; //file names
    if(argc<3 ||argc>5){ //check argument amount validity
        cerr<<"Invalid argument amount";
        return 1;
    }
    if(argv[1][0]<'0' ||argv[1][0]>'9'){ //check first argument validity
        cerr<<"1st argument must be a choice number [1-7]";
        return 1;
    }
    ppm_file_name1=argv[2]; //get file name
    if (argc>3) {
        ppm_file_name2=argv[3]; //get 2nd file name if exists
        if (argc==5) ppm_file_name3=argv[4]; //get 3rd file name if exists
    }
    
    if (argv[1][0]=='1'){ //test addition, get each pixels from file 1 and file 2, add them and write the sum to file 3
        if(!check_file(ppm_file_name2) || !check_file(ppm_file_name3)) return 1; //if file 2 and file 3 is not created exit
        test_addition(ppm_file_name1, ppm_file_name2, ppm_file_name3);
    }else if (argv[1][0]=='2'){ //test subtraction, get each pixels from file 1 and file 2, subtract them and write the result to file 3
        if(!check_file(ppm_file_name2) || !check_file(ppm_file_name3)) return 1; //if file 2 and file 3 is not created exit
        test_subtraction(ppm_file_name1, ppm_file_name2, ppm_file_name3);
    }else if (argv[1][0]=='3'){ //swap red and blue channels of the image and write the new version into file 2
        ppmImage obj;
        if (read_ppm(ppm_file_name1,obj)){
            swap_channels(obj,2);
            write_ppm(ppm_file_name2,obj);
        }else return 1; //if source file doesnt exist exit
    }else if (argv[1][0]=='4'){ //swap green and blue channels of the image and write the new version into file 2
        ppmImage obj;
        if(read_ppm(ppm_file_name1,obj)){
            swap_channels(obj,3);
            write_ppm(ppm_file_name2,obj);
        }else return 1; //if source file doesnt exist exit
    }else if (argv[1][0]=='5'){ //set green and blue channels to 0 and write the new version into file 2
        ppmImage obj;
        if(read_ppm(ppm_file_name1,obj)){
            ppmImage obj1 = single_color(obj,1);
            write_ppm(ppm_file_name2,obj1);
        }else return 1; //if source file doesnt exist exit
    }else if (argv[1][0]=='6'){ //set red and blue channels to 0 and write the new version into file 2
        ppmImage obj;
        if(read_ppm(ppm_file_name1,obj)){
            ppmImage obj1 = single_color(obj,2);
            write_ppm(ppm_file_name2,obj1);
        }else return 1; //if source file doesnt exist exit
    }else if (argv[1][0]=='7'){ //set green and red channels to 0 and write the new version into file 2
        ppmImage obj;
        if(read_ppm(ppm_file_name1,obj)){
            ppmImage obj1 = single_color(obj,3);
            write_ppm(ppm_file_name2,obj1);
        }else return 1; //if source file doesnt exist exit
    }else{ //if invalid choice number entered give error and exit
        cerr<<"Invalid choice number";
        return 1; 
    }
    //test_print(ppm_file_name1); //print file1 to terminal using test_print (it was not used in the functions above so i added at the end of main to test it)
    return 0; 
}

bool check_file(const string filename){
    ifstream test(filename);
    if (!test){ //if file with name "filename" cant be opened then file doesnt exist, return false
        cerr<<"Failed to open file "<<filename<<endl;
        return false;
    }
    test.close();
    return true;
}

int read_ppm(const string source_ppm_file_name, ppmImage& destination_object){
    int width, height, max_color;
    string format;
    ifstream read(source_ppm_file_name); //open read file
    if(!read){ //if no file
        cerr<<"Failed to open file "<<source_ppm_file_name<<endl;
        return 0;
    }
    read >> format >> width >> height >> max_color; //format: image format(p3), width:columns, height: rows, max_color: max rgb values
    if (format != "P3") return 0;
    if (!height || !width) return 0;
    vector<vector<vector<int>>> vec(height, vector<vector<int>>(width, vector<int>(3)));
            for (int i=0; i<height; i++)
                for (int j=0; j<width;j++)
                    for(int k=0; k<3; k++)
                        read >> vec[i][j][k];
    read.close();
    destination_object = ppmImage(height,width,max_color,vec); //call user-defined constructor for destination_object
    return 1;
}

int write_ppm(const string destination_ppm_file_name, const ppmImage& source_object){
    ofstream write(destination_ppm_file_name);//open write file
    if (!write){ //if file cant be created
        return 0;
    }
    write << "P3" <<endl << source_object.getHeight() << " "<< source_object.getWidth() << endl << source_object.getMaxColor() <<endl;//write format heighy width and max_color values first
    for (int i=0; i<source_object.getHeight(); i++){ //write the rgb values for each pixel
        for (int j=0; j<source_object.getWidth();j++){
            write << source_object.pixelInfo(i,j,1) <<" "; //get reference of red channel in height: i, width: j
            write << source_object.pixelInfo(i,j,2) <<" "; //get reference of green channel in height: i, width: j
            write << source_object.pixelInfo(i,j,3) <<" "; //get reference of blue channel in height: i, width: j
        }
        write<<endl;
    }
    write.close();
    return 1;
}

int test_addition(const string filename_image1, const string filename_image2, const string filename_image3){
    ppmImage img1(filename_image1); //open the file named filename_image1
    ppmImage img2(filename_image2); //open the file named filename_image2
    ppmImage img3 = img1+img2; //new object that equals to addition of img1 and img2
    img3.saveImage(filename_image3); // save the new object into file named filename_image3
    return 1;
}

int test_subtraction(const string filename_image1, const string filename_image2, const string filename_image3){
    ppmImage img1(filename_image1); //open the file named filename_image1
    ppmImage img2(filename_image2); //open the file named filename_image2
    ppmImage img3 = img1-img2; //new object that equals to subtraction of img1 and img2
    img3.saveImage(filename_image3); // save the new object into file named filename_image3
    return 1;
}

int test_print(const string filename_image1){
    ppmImage img(filename_image1);  // open the file named filename_image1
    cout<<img; //print everything in it by using << operator overloading
    return 1;
}

int swap_channels(ppmImage& image_object_to_be_modified, int swap_choice){
    if (swap_choice<1 || swap_choice>3) return 1; //no swaps
    int height = image_object_to_be_modified.getHeight();
    int width = image_object_to_be_modified.getWidth();
    for (int i=0; i< height; i++){
        for(int j=0; j< width; j++){
            int& r= image_object_to_be_modified(i,j,1); //store r channel in height: i , width: j
            int& g= image_object_to_be_modified(i,j,2); //store g channel in height: i , width: j
            int& b= image_object_to_be_modified(i,j,3);//store b channel in height: i , width: j
            if (swap_choice==1) swap(r,g); 
            else if (swap_choice==2) swap(r,b);
            else if (swap_choice==3) swap(g,b);
        }
    }
    return 1;
}

ppmImage single_color(const ppmImage& source, int color_choice){
    int height = source.getHeight(), width= source.getWidth(), max_color= source.getMaxColor();
    vector<vector<vector<int>>> vec(height, vector<vector<int>>(width, vector<int>(3)));
    for (int i=0; i<height; i++){
        for (int j=0; j<width; j++){
            if(color_choice==1){ //set g and b channels to 0
                vec[i][j][0]=source(i,j,1); 
                vec[i][j][1]=0;
                vec[i][j][2]=0;
            }
            if(color_choice==2){ //set r and b channels to 0
                vec[i][j][1]=source(i,j,2);
                vec[i][j][0]=0;
                vec[i][j][2]=0;
            }
            if(color_choice==3){ //set g and r channels to 0
                vec[i][j][2]=source(i,j,3);
                vec[i][j][1]=0;
                vec[i][j][0]=0;
            }
        }
    }
    return ppmImage(height, width, max_color, vec); //call user-defined constructor to create new object
}

void ppmImage::openImage(const string& name){ //name = file name
    if(!check_file(name)) exit(5);
    ifstream read(name); //open read file
    read >> format >> width >> height >> max_color; //format: image format(p3), width:columns, height: rows, max_color: max rgb values
    if (format != "P3"){ //we work with p3 only so if not p3 give error 
        cerr<<"File is not in ASCII PPM format."<<endl;
        exit(3);
    }
    if (!height || !width){ //if height or width is null give error
        cerr<<"The file is not valid."<<endl;
        exit(4);
    }
    //initialize and fill the vector with read values from the file, each cell represents one of r g b channel of pixel
    vector<vector<vector<int>>> vec(height, vector<vector<int>>(width, vector<int>(3))); 
    for (int i=0; i<height; i++)
        for (int j=0; j<width;j++)
            for(int k=0; k<3; k++)
                read >> vec[i][j][k];

    read.close();
    pixels =vec;
}

void ppmImage::saveImage(const string& name)const{//name=file name
    ofstream write(name);//open write file
    if (!write){ //if file cant be created
        cerr<<"File could not be created"<<endl;
        exit(5); 
    }
    write << format <<endl << height << " "<< width << endl << max_color <<endl;//write format heighy width and max_color values first
    for (int i=0; i<height; i++){ //write the rgb values for each pixel
        for (int j=0; j<width;j++)
            for(int k=0; k<3; k++)
                write << pixels[i][j][k] <<" ";
        write<<endl;
    }
    write.close();
}

int ppmImage::pixelInfo(int h, int w, int c)const{ //if c==1: return r channel, c==2: return g channel, c==3: return b channel.
    if(h>=height || h<0 || w>=width || w<0 || c<1 || c>3){
        cerr<<"Invalid index";
        exit(6);
    }
    return pixels[h][w][c-1];
}

ppmImage ppmImage::operator+(const ppmImage& other)const{
    if(height!=other.height || width!=other.width) return ppmImage();//height and width of files must be equal, if not return empty object
    //add each pixel and save the sum into vec
    vector<vector<vector<int>>> vec(height, vector<vector<int>>(width, vector<int>(3)));
    for (int i=0; i<height; i++){
        for (int j=0; j<width;j++){
            for(int k=0; k<3; k++){
                vec[i][j][k] = pixels[i][j][k]+other.pixels[i][j][k]; 
                if (vec[i][j][k]>max_color) vec[i][j][k]=max_color;
            }
        }
    }
    return ppmImage(height,width,max_color,vec); //user-defined constructor to return obj
}

ppmImage ppmImage::operator-(const ppmImage& other)const{
    if(height!=other.height || width!=other.width) return ppmImage(); //height and width of files must be equal, if not return empty object
    //subtract each pixel and save the result into vec
    vector<vector<vector<int>>> vec(height, vector<vector<int>>(width, vector<int>(3)));
    for (int i=0; i<height; i++){
        for (int j=0; j<width;j++){
            for(int k=0; k<3; k++){
                vec[i][j][k] = pixels[i][j][k]-other.pixels[i][j][k];
                if (vec[i][j][k]<0) vec[i][j][k]=0;
            }
        }
    }
    return ppmImage(height,width,max_color,vec); //user-defined constructor to return obj
}

ostream& operator<<(ostream& output, const ppmImage& cl){ //<< operator overloading to print to terminal
    output<<cl.format<<endl<<cl.width<<" "<<cl.height<<endl<<cl.max_color<<endl;
    for(int h=0; h<cl.height; h++){
        for(int w=0; w<cl.width; w++){
            output<<cl.pixels[h][w][0]<<" "<<cl.pixels[h][w][1] <<" "<<cl.pixels[h][w][2]<<" "; //r g b
        }
        output<<endl;
    }
    return output;
}

int& ppmImage::operator()(int h, int w, int c){ //() operator overloading to return the reference of any of r g b channels in a specific index, 
//if c==1: return r channel, c==2: return g channel, c==3: return b channel.
//the difference of this with pixelInfo is, this returns a reference so we can make changes.
    if(h>height || h<0 || w>width || w<0){
        cerr<<"Invalid index";
        exit(6);
    }
    if (c>3 || c<0){
        cerr<<"Invalid color code";
        exit(7);
    }
    return pixels[h][w][c-1];
}

const int& ppmImage::operator()(int h, int w, int c)const{ //() operator overloading for const variables
    if(h>=height || h<0 || w>=width || w<0){
        cerr<<"Invalid index";
        exit(6);
    }
    if (c>3 || c<0){
        cerr<<"Invalid color code";
        exit(7);
    }
    return pixels[h][w][c-1];
}