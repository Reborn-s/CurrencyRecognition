function [ mFiles,filePaths ] = dfsFolder( strPath, extName )  
    %���������飬�ֱ𱣴��ļ���·��  
    mFiles = cell(0,0);  
    mPath  = cell(0,0);  
	filePaths = {};
      
    mPath{1}=strPath;  
    [r,c] = size(mPath);  
    while c ~= 0  
        strPath = mPath{1};  
        Files = dir(strPath);  
        LengthFiles = length(Files);  
        if LengthFiles == 0  
            break;  
        end  
        mPath(1)=[];  
        iCount = 1;  
        while LengthFiles>0  
            if Files(iCount).isdir==1  
                if (strcmp(Files(iCount).name,'.')~=1)&&(strcmp(Files(iCount).name,'..')~=1)
					filePath = [strPath  Files(iCount).name '/'];  
                    [r,c] = size(mPath);  
                    mPath{c+1}= filePath; 
                end  
            else  
				full_name=[strPath,Files(iCount).name];  
				[pathstr,name,ext,versn]=fileparts(full_name);
				if(strcmp(ext,extName))
					filePath = [strPath  Files(iCount).name];  
					[row,col] = size(mFiles);  
					mFiles{col+1}=Files(iCount).name;  
					filePaths{col+1} = filePath;
				end
					
            end  
  
            LengthFiles = LengthFiles-1;  
            iCount = iCount+1;  
        end  
        [r,c] = size(mPath);  
    end  

end  
