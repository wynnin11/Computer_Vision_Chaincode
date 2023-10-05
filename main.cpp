#include <iostream>
#include <fstream>
class ChainCode {
private:
    struct Point{
        int row;
        int col;
    };
    struct CCproperty{
        int label;
        int numpixels;
        int minRow;
        int maxRow;
        int minCol;
        int maxCol;
    };
    int numCC, numRows, numCols, minVal, maxVal;
    int** imageAry;
    int** boundaryAry;
    int** CCAry;
    Point coordOffset[8] { {0, +1}, {-1, +1}, {-1, 0}, {-1, -1}, {0, -1}, {+1, -1}, {+1, 0}, {+1, +1} };
    Point neightborCoord[8];
    int zeroTable[8] {6, 0, 0, 2, 2, 4, 4, 6};
    Point startP;
    Point currentP;
    Point nextP;
    int lastQ;
    int nextDir;
    int PchainDir;
    CCproperty CC;
public:
    //constructor
    ChainCode(){}
    //member functions
    void zeroFramed();
    void loadImage(std::ifstream &file, int** ary){
        for(int i = 1; i <= numRows; i++){
            for(int j = 1; j <= numCols; j++){
                file >> ary[i][j];
            }
        }
    }
    void clearCCAry(){
        for(int i = 0; i < numRows+2; i++){
            for( int j = 0; j < numCols+2; j++){
                CCAry[i][j] = 0;
            }
        }
    }
    void loadCCAry(int label, int** CCAry){
        for(int i = 0;i < numRows;i++){
            for(int j = 0; j < numCols; j++){
                if(imageAry[i][j] == label){
                    CCAry[i][j] = label;
                }
            }
        }
    }
    void getChainCode(CCproperty CC, int** CCAry,std::ofstream& ChainCodeFile){
        //step 1:
        int label = CC.label;
        //step 2:
        int iRow = 0;
        int jCol = 0;
        bool found = false;
        while((iRow < numRows+2) && (found !=true)){
            jCol = 0;
            while((jCol < numCols+2) && (found != true) ){
                if(CCAry[iRow][jCol] == label){
                    ChainCodeFile << label <<" "<< iRow << " " << jCol<< " ";
                    setStartP(iRow,jCol);
                    setCurrentP(iRow,jCol);
                    setLastQ(4);
                    found = true;
                }
                jCol++;
            }
            iRow++;
        }
        //step 8:
        do{
            //step 3:
            int nextQ = (lastQ+1)%8;
            //step 4:
            PchainDir = findNextP(currentP,nextQ);
            nextP = neightborCoord[PchainDir];
            CCAry[currentP.row][currentP.col] = -label;
            //step 5:
            ChainCodeFile << PchainDir <<" ";
            //step 6:
            if (PchainDir == 0){
                lastQ = zeroTable[7];
            }
            else{
                lastQ = zeroTable[PchainDir-1];
            }
            //step 7:
            currentP = nextP;
        }while(currentP.row != startP.row || currentP.col != startP.col);
    }
    void loadNeighborsCoord(Point currentP){
        for(int i = 0; i < 8; i++){
            neightborCoord[i].row = coordOffset[i].row + currentP.row;
            neightborCoord[i].col = coordOffset[i].col + currentP.col;
    
        }
    }
    int findNextP(Point currentP, int lastQ){
        //step 0:
        loadNeighborsCoord(currentP);
        //step 1:
        int index = lastQ;
        bool found = false;
        //step 5:
        while(found != true){
            //step 2:
            int iRow = neightborCoord[index].row;
            int jCol = neightborCoord[index].col;
            //step 3:
            if(imageAry[iRow][jCol] == CC.label){
                PchainDir = index;
                found = true;
            }
            //step 4:
            index = (index+1)%8;
        }
        return PchainDir;
    }
    void constructBoundary(std::fstream &chainFile){
        int label, iRow, jCol, dir,startR,startC;
        int temp;
        chainFile >> temp;
        chainFile >> temp;
        chainFile >> temp;
        chainFile >> temp;
        //numCC
        chainFile >> temp;
        while(numCC != label){
        chainFile >> label;
        chainFile >> startR >> startC;
        iRow = startR;
        jCol = startC;
            do{
                boundaryAry[iRow][jCol] = label;
                chainFile >> dir;
                iRow = coordOffset[dir].row + iRow;
                jCol = coordOffset[dir].col + jCol;
            } while(startR != iRow || startC != jCol);
        }
    }
    void reformatPrettyPrint(int** aryTwo,std::ofstream& file){
        file<<numRows<<" "<<numCols<<" "<< minVal<<" "<< maxVal<<std::endl;
        for(int i = 0;i < numRows + 2; i++){
            for(int j = 0; j < numCols + 2; j++){
                if(aryTwo[i][j] > 0){
                   file<<aryTwo[i][j]<<" ";
                }
                else{
                    file<<". ";
                }
            }
            file<<std::endl;
        }
    }
    //setters
    void setNumCC(int i){numCC=i;}
    void setNumRows(int i){ numRows=i;}
    void setNumCols(int i){ numCols=i;}
    void setMinVal(int i){ minVal=i;}
    void setMaxVal(int i){ maxVal=i;}
    void setLastQ(int i){ lastQ=i;}
    void setNextDir(int i){ nextDir=i;}
    void setPchainDir(int i){ PchainDir=i;}
    void setImageAry(){
        imageAry = new int*[numRows+2];
        for(int i = 0; i < numRows+2; i++){
            imageAry[i] = new int[numCols+2];
        }
    }
    void setCCAry(){
        CCAry = new int*[numRows+2];
        for(int i = 0; i < numRows+2; i++){
            CCAry[i] = new int[numCols+2];
        }
    }
    void setStartP(int i, int j){
        startP.row = i;
        startP.col = j;
    }
    void setCurrentP(int i, int j){
        currentP.row = i;
        currentP.col = j;
    }
    void setNextQ(int i, int j){
        
    }
    void setboundaryAry(){
        boundaryAry = new int*[numRows+2];
        for(int i = 0; i < numRows+2; i++){
            boundaryAry[i] = new int[numCols+2];
        }
    }
    //getters
    int getNumCC(){return numCC;}
    int getNumRows(){return numRows;}
    int getNumCols(){return numCols;}
    int getMinVal(){return minVal;}
    int getMaxVal(){return maxVal;}
    CCproperty& getCC(){return CC;}
    int** getImageAry(){return imageAry;}
    int** getBoundaryAry(){return boundaryAry;}
    int** getCCAry(){return CCAry;}
    Point* getCoorOffset(){return coordOffset;}
    Point* getNeightborCoord(){return neightborCoord;}
    int* getZeroTable(){return zeroTable;}
    Point getStartP(){return startP;}
    Point getCurrentP(){return currentP;}
    Point getNextP(){return nextP;}
    int getLastQ(){return lastQ;}
    int getNextDir(){return nextDir;}
    int getPchainDir(){return PchainDir;}
    void viewAry(int** ary){
        for(int i = 0; i <numRows+2;i++){
            for(int j = 0; j< numCols+2;j++){
                std::cout<<ary[i][j]<<" ";
            }
            std::cout<<std::endl;
        }
    }
};

int main(int argc, const char * argv[]) {
    
    std::ifstream labelFile(argv[1]);
    std::ifstream propFile(argv[2]);
   
    ChainCode Image;
    //step 0:
    int temp;
    labelFile >> temp;
    propFile >> temp;
    Image.setNumRows(temp);
    propFile >> temp;
    labelFile >> temp;
    Image.setNumCols(temp);
    propFile >> temp;
    labelFile >> temp;
    Image.setMinVal(temp);
    propFile >> temp;
    labelFile >> temp;
    Image.setMaxVal(temp);
    propFile >> temp;
    Image.setNumCC(temp);
    Image.setImageAry();
    Image.setboundaryAry();
    Image.loadImage(labelFile,Image.getImageAry());
    Image.setCCAry();
    //step 1:
    std::string chainCodeFileName = argv[1];
    chainCodeFileName.append("_chainCode.txt");
    std::string BoundaryFileName = argv[1] ;
    BoundaryFileName.append("_Boundary.txt");
    std::ofstream chainCodeFile(chainCodeFileName);
    std::ofstream BoundaryFile(BoundaryFileName);
    chainCodeFile << Image.getNumRows()<<" "<<Image.getNumCols()<<" "<<Image.getMinVal()<<" "<<Image.getMaxVal()<<std::endl;
    chainCodeFile << Image.getNumCC()<<std::endl;
    //step 6:
    
    while(Image.getNumCC() != Image.getCC().label){
        //step 2:
        propFile >> Image.getCC().label;
        propFile >> Image.getCC().numpixels;
        propFile >> Image.getCC().minRow;
        propFile >> Image.getCC().minCol;
        propFile >> Image.getCC().maxRow;
        propFile >> Image.getCC().maxCol;
        //step 3:
        Image.clearCCAry();
        //step 4:
        Image.loadCCAry(Image.getCC().label, Image.getCCAry());
        //step 5:
        Image.getChainCode(Image.getCC(), Image.getCCAry(), chainCodeFile);
        chainCodeFile<<std::endl;
    }
    //step 7:
    chainCodeFile.close();
    //step 8:
    std::fstream chainCode;
    //chainCode.open(chainCodeFileName);
    chainCode.open(chainCodeFileName);
    //step 9:
    Image.constructBoundary(chainCode);
    Image.reformatPrettyPrint(Image.getBoundaryAry(),BoundaryFile);
    
    //step 10
    chainCodeFile.close();
    labelFile.close();
    propFile.close();
    BoundaryFile.close();
    
    return 0;
    
}
