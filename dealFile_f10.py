
import os


#path = "./GLCMdata/f10d5/all/FY/"
#outpath = "./newall/f10d5/FY/"

path = "./GLCMdata/f10d5/single/FN/"
outpath = "./matlabTestcase/f10d5/FN/"
featureKind = 10

def dealEachFile(root,fileName):
    ans = {}
    try:
        fi = open(root+'/'+fileName,"r+")
    except:
        return ans

    if not fi:
        return ans
    picCount = 1;
    locationCnt = 0;
    #allFeatures = [feature1,feature2,feature3,feature4,feature5]
    content = fi.read()
    lines = content.split("\n")
    locate = 0
    allLocations = []
    for line in lines:
        line = line.strip()
        if picCount==1 and line!="" :
            elem = line.split("\t")
            locationCnt+=1
            feature1 = [];
            feature2 = [];
            feature3 = [];
            feature4 = [];
            feature5 = [];
            feature6 = [];
            feature7 = [];
            feature8 = [];
            feature9 = [];
            feature10 = [];
            newLocation = [feature1,feature2,feature3,feature4,feature5,feature6,feature7,feature8,feature9,feature10]
            for f in range(featureKind):
                newLocation[f].append(float(elem[f]))
            allLocations.append(newLocation)
        elif line=="":
            picCount+=1
            locate = 0
        elif picCount!=1 and line!="":
            location = allLocations[locate]
            elem = line.split("\t")
            for f in range(featureKind):
                location[f].append(float(elem[f]))
            locate+=1

    writeFile(picCount,fileName,allLocations)
    fi.close()

def dealFile():
    for root,dirs,files in os.walk(path):
        for onefile in files:
            dealEachFile(root,onefile)


def writeFile(picCount,filename,allLocations):
    #fo = open(outpath+"new"+filename.replace('-','.'),"w+")
    fo = open(outpath + filename.replace('-', '.'), "w+")
    for i in range(len(allLocations)-1):
        location = allLocations[i]
        for f in range(featureKind):
            feature = location[f]
            for j in range(len(feature)):
                fo.write(str(feature[j])+" ")
            fo.write("\n")
        fo.write("\n")
    location = allLocations[len(allLocations)-1]
    for f in range(featureKind-1):
        feature = location[f]
        for j in range(len(feature)):
            fo.write(str(feature[j]) + " ")
        fo.write("\n")
    feature = location[featureKind-1]
    for j in range(len(feature)):
        fo.write(str(feature[j]) + " ")
    fo.close()
    print picCount,"\n"


def mainfunc():
    dealFile()

mainfunc()