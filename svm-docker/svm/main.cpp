//******  This program is for libSVM training & testing ******
//content: main.cpp svm.cpp svm.h grid.py
//dependencies: install two libraries : (1)opencv 
//              (2)libsvm, replace grid.py in libsvm & edit the path of grid.py in line 839: "/home/cks/libsvm-3.20/tools/grid.py ";
//compile: g++ --std=c++11 main.cpp svm.cpp -lopencv_core -o main 
//run: (1)K-folidng; (2)Do_subsample(0=false, 1=yes); (3)feature_filename.txt(filename 1:f1 2:f2 ...); (4)label.txt 
//        ex: ./main 10 1 OM_Caffe.txt OM_caffe_label.txt 
//output: "SVM folding result.txt"
//******Author: HTC SE CK 

#include <opencv2/opencv.hpp>
#include <fstream>
#include "svm.h"

using namespace cv; // OpenCV API is in the C++ "cv" namespace
using namespace std;

//////////SVM////////////////////////////////////////
//struct svm_problem prob;      // set by read_problem
struct svm_parameter param;     // set by parse_command_line
///////////////////////////////////////////////////

//
#define NUMBER_OF_CLASSES 2
#define LIBSVM_PATH   "/opt/libsvm/" 
//"/home/cks/libsvm-3.20/" //
// N.B. classes are integer handwritten digits in range 0-9
vector<vector<double> > vector_0; 
vector<vector<double> > vector_1;
vector<vector<double> > vector_2;
vector<vector<double> > vector_3;
vector<string> vector_filename_0;
vector<string> vector_filename_1;
vector<string> vector_filename_2;
vector<string> vector_filename_3;

void clear_vectors()
{
    vector_0.clear();
    vector_1.clear();
    vector_2.clear();
    vector_3.clear();
    vector_filename_0.clear();
    vector_filename_1.clear();
    vector_filename_2.clear();
    vector_filename_3.clear();
}

vector<string> split(string& str,const char* c) //function
{
    char *cstr, *p;
    vector<string> res;
    cstr = new char[str.size()+1];
    strcpy(cstr,str.c_str());
    p = strtok(cstr,c);
    while(p!=NULL)
    {
        res.push_back(p);
        p = strtok(NULL,c);
    }

    return res;
}

//read file include all data for train & test => store in vector_0 = class_0 , vector_1 = class_1 ..... format: diagnosis f1:XX f2:XX f3:XX f4:XX.....
void read_alldata_tovector_svm(String filename)
{
    string s;
    float t;
    fstream fin;
    fin.open(filename,ios::in);
    if(!fin)
        cout << " fail to read file " << endl;
    else
    {
        //while(!fin.eof())
        /*
        (0) = diagnosis
        Others ... features
        */
        string s;
        while(!fin.eof())
        {
            getline(fin,s);
	    s.erase(s.find_last_not_of(" \n\r\t")+1);
            vector<string> ss = split(s," ");
            if(ss.size() == 0)
                break;

            if(strcmp(ss.at(0).c_str(),"0") == 0)
            {
                vector<double> tmp;

                for(int i = 1;i<ss.size();i++)
                {
                    vector<string> sss = split(ss.at(i),":");// 1:XX 
                    tmp.push_back(atof(sss.at(1).c_str()));
                }
                vector_0.push_back(tmp);
                vector_filename_0.push_back(ss.at(0));
            }
            else if(strcmp(ss.at(0).c_str(),"1") == 0)
            {
                vector<double> tmp;

                for(int i = 1;i<ss.size();i++)
                {
                    vector<string> sss = split(ss.at(i),":");// 1:XX 
                    tmp.push_back(atof(sss.at(1).c_str()));
                }
                vector_1.push_back(tmp);
                vector_filename_1.push_back(ss.at(0));
            }
            else if(strcmp(ss.at(0).c_str(),"2") == 0)
            {
                vector<double> tmp;

                for(int i = 1;i<ss.size();i++)
                {
                    vector<string> sss = split(ss.at(i),":");// 1:XX 
                    tmp.push_back(atof(sss.at(1).c_str()));
                }
                vector_2.push_back(tmp);
                vector_filename_2.push_back(ss.at(0));
            }
            else if(strcmp(ss.at(0).c_str(),"3") == 0)
            {
                vector<double> tmp;

                for(int i = 1;i<ss.size();i++)
                {
                    vector<string> sss = split(ss.at(i),":");// 1:XX 
                    tmp.push_back(atof(sss.at(1).c_str()));
                }
                vector_3.push_back(tmp);
                vector_filename_3.push_back(ss.at(0));
            }
        }
    }
}
void read_alldata_tovector(String filename)//store in vector_0 = class_0 , vector_1 = class_1 ..... format: diagnosis f1 f2 f3 f4.....
{
    float t;
    fstream fin;
    fin.open(filename,ios::in);
    if(!fin)
        cout << " fail to read file " << endl;
    else
    {
        //while(!fin.eof())
        /*
        (0) = diagnosis
        Others ... features
        */
        string s;
        while(!fin.eof())
        {
            getline(fin,s);
	    s.erase(s.find_last_not_of(" \n\r\t")+1);
            vector<string> ss = split(s," ");
            if(ss.size() == 0)
                break;

            if(strcmp(ss.at(0).c_str(),"0") == 0)
            {
                vector<double> tmp;

                for(int i = 1;i<ss.size();i++)
                { 
                    tmp.push_back(atof(ss.at(i).c_str()));
                }
                vector_0.push_back(tmp);
                vector_filename_0.push_back(ss.at(0));
            }
            else if(strcmp(ss.at(0).c_str(),"1") == 0)
            {
                vector<double> tmp;

                for(int i = 1;i<ss.size();i++)
                {
                    tmp.push_back(atof(ss.at(i).c_str()));
                }
                vector_1.push_back(tmp);
                vector_filename_1.push_back(ss.at(0));
            }
            else if(strcmp(ss.at(0).c_str(),"2") == 0)
            {
                vector<double> tmp;

                for(int i = 1;i<ss.size();i++)
                {
                    tmp.push_back(atof(ss.at(i).c_str()));
                }
                vector_2.push_back(tmp);
                vector_filename_2.push_back(ss.at(0));
            }
            else if(strcmp(ss.at(0).c_str(),"3") == 0)
            {
                vector<double> tmp;

                for(int i = 1;i<ss.size();i++)
                {
                    tmp.push_back(atof(ss.at(i).c_str()));
                }
                vector_3.push_back(tmp);
                vector_filename_3.push_back(ss.at(0));
            }
        }
    }
}

//vector<int> random_pick(int number,Mat input,Mat output)
//{
//  vector<int> pick_item;
//  if(number > input.rows)
//  {
//      cout << "error @ random_pick : number > input.rows" << endl;
//      system("pause");
//  }
//  else
//  {
//      srand (time(NULL));
//      int i = 0;
//      while(i < number)
//      {
//          int current_number = rand()%input.rows;
//          vector<int>::iterator it = find(pick_item.begin(), pick_item.end(), current_number);
//
//          if(it != pick_item.end()) {
//              //cout << "§äšì·jŽM­È¡I" << endl;
//          }
//          else 
//          {
//              //cout << "§ä€£šì·jŽM­È¡I" << endl;
//              pick_item.push_back(current_number);
//              output.row(i) = input.row(current_number);
//              i++;
//          }
//      }
//  }
//  return pick_item;
//}

void set_svm_para(double c,double g)
{
    // default values
    param.svm_type = C_SVC;
    param.kernel_type = RBF;//RBF LINEAR POLY  SIGMOID
    param.degree = 3;
    param.gamma = g;    // 1/num_features
    param.coef0 = 0;
    param.nu = 0.5;
    param.cache_size = 100; 
    param.C = c;
    param.eps = 1e-3;
    param.p = 0.1;
    param.shrinking = 1;
    param.probability = 1; 

    ////balance data
    param.nr_weight = 0;
    param.weight_label = NULL;
    param.weight = NULL;

    ////unbalanced data 
    //param.nr_weight = 2;//number of elements in the array weight_label and weight
    //int* label; 
    //label = new int[param.nr_weight];
    //label[0] = 3; label[1] = 1;
    //param.weight_label = label;//Each weight[i] corresponds to weight_label[i], meaning that the penalty of class weight_label[i] is scaled by a factor of weight[i]
    //double* weight;
    //weight = new double[param.nr_weight];
    //weight[0] = 10; weight[1] = 1;
    //param.weight = weight;

    //cross_validation = 0;
}

void To_svm(string inputfilename,string outputfilename)//format To SVM function: filename Diagnosis f1 f2 ... -> Diagnosis 1:f1 2:f2 ...
{
    fstream fin(inputfilename.c_str(),ios::in);
    fstream fout(outputfilename.c_str(),ios::out);
    string s;
    while(!fin.eof())
    {
        getline(fin,s);
	s.erase(s.find_last_not_of(" \n\r\t")+1);
        vector<string> ss = split(s," ");
        if(ss.size() == 0)
            break;

        fout << ss.at(1) << " ";//Diagnosis
        for(int i = 2;i<ss.size();i++)
        {
            fout << i-1 << ":" << ss.at(i) << " ";
        }
        fout << endl;
    }
    fin.close();
    fout.close();
}



double value_scale(double value,double MAX_value,double MIN_value,double lower_bound,double upper_bound)
{
    if(MAX_value == MIN_value)
    {
        cout << "warning @value_scale, MAX_value == MIN_value " << endl;
        return value;
    }
    return (value-MIN_value)*(upper_bound-lower_bound)/(MAX_value-MIN_value) + lower_bound;
}

void SVM_scale(vector<string> input_filename,double lower_bound,double upper_bound,vector<string> outputfilename)
{
    //1. read file, format = diagnosis feature1 2 3... ; 
    vector<double> Max_vector;//record the max of each term
    vector<double> Min_vector;//record the min of each term
    cout << input_filename.at(0) << endl;  
    //2.
    for(int i = 0; i < input_filename.size(); i++ )
    {
        ifstream fin(input_filename.at(i));
        if(!fin)
        {
            cout << "can't read " << input_filename.at(i) << endl;
        }
        string s;
        //record max/min value
        while(!fin.eof())
        {
            vector<double> tmp;
            getline(fin,s);
	    s.erase(s.find_last_not_of(" \n\r\t")+1);
            vector<string> ss = split(s," ");
            if(ss.size() == 0)
                break;
            if(Max_vector.empty())
            {
                for(int i = 0;i < ss.size();i++)
                {
                    Max_vector.push_back(-DBL_MAX);
                    Min_vector.push_back(DBL_MAX);
                }
            }
            for(int j = 0;j < ss.size();j++)// 
            {

                if(j == 0 )
                    ;
                else
                {
                    vector<string> sss = split(ss.at(j),":");

                    double value = atof(sss.at(1).c_str());

                    if(value > Max_vector.at(j))
                        Max_vector.at(j) = value;
                    if(value < Min_vector.at(j))
                        Min_vector.at(j) = value;
                    tmp.push_back(value);
                    //cout << value << " ";
                }
            }
        }
        fin.close();
    }

    cout << "Max_vector.size()=" << Max_vector.size()-1 << endl;
    //for(int i = 0; i < Max_vector.size(); i++)
    //cout << Max_vector.at(i) << "/" << Min_vector.at(i) << " ";

    //3.scale & output
    for(int i = 0; i < input_filename.size(); i++ )
    {
        ifstream fin2(input_filename.at(i));
        fstream fout(outputfilename.at(i),ios::out);
        string s;
        while(!fin2.eof())
        {
            vector<double> tmp;
            getline(fin2,s);
	    s.erase(s.find_last_not_of(" \n\r\t")+1);
            vector<string> ss = split(s," ");
            if(ss.size() == 0)
                break;
            for(int j = 0;j < ss.size();j++)
            {
                if(j == 0)
                {
                    double value = atof(ss.at(j).c_str());
                    fout << value << " ";//diagnosis label
                }
                else
                {

                    vector<string> sss = split(ss.at(j),":");
                    double value = atof(sss.at(1).c_str());

                    if(Max_vector.at(j) == Min_vector.at(j))
                        fout << value << " ";
                    else
                        fout << value_scale(value,Max_vector.at(j),Min_vector.at(j),lower_bound,upper_bound) << " ";
                }
            }
            fout << endl;
        }
        fin2.close();
        fout.close();
    }

    //4. output scale configuration file
    string scale_filename = outputfilename.at(0) + string("_scaleconfig.txt");
    fstream fout2 (scale_filename,ios::out);
    for(int i = 0; i < Max_vector.size();i++)
    {
        fout2 << Max_vector.at(i) << " ";
    }
    fout2 << endl;
    for(int i = 0; i < Min_vector.size();i++)
    {
        fout2 << Min_vector.at(i) << " ";
    }
    fout2 << endl;
    fout2.close();
}

void Relabel(string inputfile,string labelfile)//input: format: filename 1:f1 2:f2... + label.txt ; output:
{
    cout << "relabeling " <<  inputfile << endl;
    //READ OTITIS DIAGNOSIS
    fstream fin(labelfile,ios::in);
    map<string,int> Diagnosis_index;
    vector<int> diagnosis_label;
    int index = 0;
    while(!fin.eof())
    {
        string s;
        fin >> s;
        Diagnosis_index.insert(pair<string,int>(s,index));
        index++;

        int i;
        fin >> i;
        diagnosis_label.push_back(i);
    }
    fin.close();

    fstream fin2(inputfile,ios::in);
    fstream fout("Relabeled.txt",ios::out);

    //Map img to Diagnosis
    int file_index = -1;
    while(!fin2.eof())
    {
        string s;
        getline(fin2,s);
	s.erase(s.find_last_not_of(" \n\r\t")+1);
        vector<string> ss = split(s," ");
        if(ss.size() == 0)
            break;

	//ss.back().erase(ss.back().length() -1);
	//if(  ss.back().length() == 1)//ss.back()[ss.back.length()-1] == '\n'||
	{
		//ss.pop_back();
		//cout << "pop_back()" << endl;
	}
        map<string,int>::iterator itr =  Diagnosis_index.find(ss.at(0));
        if(itr != Diagnosis_index.end())
        {
            fout << diagnosis_label.at(itr->second) << " ";
            for(int i = 1; i < ss.size();i++) // -1, since the last one is "\n", so don't process it 
            {
                fout << ss.at(i) << " ";
            }
            fout << endl;
        }
        else
        {
            cout << "error " << ss.at(0) << endl;
        }
    }
    fin2.close();
    fout.close();
}

int sub_sample_(string filename,string outfilename) //for OM_caffe_label
{

    vector<string> vector_0,vector_1;
    fstream fin(filename,ios::in);

    while(!fin.eof())
    {
        string s;
        getline(fin,s);
	s.erase(s.find_last_not_of(" \n\r\t")+1);
        vector<string> tmp = split(s," ");
        if( tmp.size() == 0 )
            break;
        cout << tmp.at(0) << "  ";
        if( strcmp( tmp.at(0).c_str() ,"0" )==0 )// || strcmp( tmp.at(0).c_str(),"2" )==0 )
        {
            stringstream ss;
            for(int i = 1; i < tmp.size();i++)
            {
                ss << tmp.at(i);
                ss << " ";
            }
            vector_0.push_back(ss.str());
        }
        else if( strcmp( tmp.at(0).c_str() ,"1" )==0 )
        {
            stringstream ss;
            for(int i = 1; i < tmp.size();i++)
            {
                ss << tmp.at(i);
                ss << " ";
            }
            vector_1.push_back(ss.str());
        }
        /*else 
        {
        cout << "error label:" << label << endl;
        system("pause");
        }*/
    }
    fin.close();

    int n_vector_0 = vector_0.size();
    int n_vector_1 = vector_1.size();


    cout << "number of vector 0 =" << n_vector_0 << "  number of vector 1 =" <<  n_vector_1 << endl;
    int subsample_num = 0;
    if( n_vector_1/n_vector_0 > n_vector_0/n_vector_1 )     
        subsample_num = n_vector_1/n_vector_0;
    else
        subsample_num = n_vector_0/n_vector_1;

    if( subsample_num >= 2 )
    {
        cout << "sub-sampling:" << n_vector_1/n_vector_0 << endl;

        for(int sub_fold_n = 0; sub_fold_n < n_vector_1/n_vector_0;sub_fold_n++)
        {
            //1.create the filename 
            stringstream ss;
            ss << outfilename;
            ss << "_";
            ss << sub_fold_n;
            ss << ".txt";

            //2. output the fewer class 
            fstream fout(ss.str().c_str(),ios::out );
            for(int i = 0; i < vector_0.size();i++)
            {
                fout << "0" << " " << vector_0.at(i) << endl;
            }


            //3. output the larger class
            if( sub_fold_n != n_vector_1/n_vector_0 -1  )
            {
                for(int i = sub_fold_n*n_vector_0 ; i < (sub_fold_n+1)*n_vector_0;i++)
                {
                    fout << "1" << " " << vector_1.at(i) << endl;
                }
            }
            else
            {
                for(int i = sub_fold_n*n_vector_0 ; i < n_vector_1;i++)
                {
                    fout << "1" << " " << vector_1.at(i) << endl;
                }
            }
            fout.close();
        }
    }
    else
    {
        cout << "don't need sub-sampling, do relabel only" << endl;
        //1.create the filename 
        stringstream ss;
        ss << outfilename;
        ss << "_";
        ss << 0;
        ss << ".txt";

        //2. output the 0 class 
        fstream fout(ss.str().c_str(),ios::out );
        for(int i = 0; i < vector_0.size();i++)
        {
            fout << "0" << " " << vector_0.at(i) << endl;
        }

        //3. output the 1 class
        for(int i = 0 ; i < n_vector_1;i++)
        {
            fout << "1" << " " << vector_1.at(i) << endl;
        }
        fout.close();
    }
}

int sub_sample(string filename ,string outfilename) // input filename format: binary_label(0 or 1) f1 f2 f3 ..... 
{
    vector<string> vector_0,vector_1;
    fstream fin(filename,ios::in);

    while(!fin.eof())
    {
        string s;
        getline(fin,s);
	s.erase(s.find_last_not_of(" \n\r\t")+1);
        vector<string> tmp = split(s," ");
        if( tmp.size() == 0 )
            break;
        cout << tmp.at(0) << "  ";
        if( strcmp( tmp.at(0).c_str() ,"0" )==0 )
        {
            stringstream ss;
            for(int i = 0; i < tmp.size();i++)
            {
                ss << tmp.at(i);
                ss << " ";
            }
            vector_0.push_back(ss.str());
        }
        else if( strcmp( tmp.at(0).c_str() ,"1" )==0 )
        {
            stringstream ss;
            for(int i = 0; i < tmp.size();i++)
            {
                ss << tmp.at(i);
                ss << " ";
            }
            vector_1.push_back(ss.str());
        }
        /*else 
        {
        cout << "error label:" << label << endl;
        system("pause");
        }*/
    }
    fin.close();

    int n_vector_0 = vector_0.size();
    int n_vector_1 = vector_1.size();


    cout << "number of vector 0 =" << n_vector_0 << "  number of vector 1 =" <<  n_vector_1 << endl;
    int subsample_num = 0;
    if( n_vector_1/n_vector_0 > n_vector_0/n_vector_1 )     
        subsample_num = n_vector_1/n_vector_0;
    else
        subsample_num = n_vector_0/n_vector_1;

    if( subsample_num >= 2 )
    {
        cout << "sub-sampling:" << n_vector_1/n_vector_0 << endl;

        for(int sub_fold_n = 0; sub_fold_n < n_vector_1/n_vector_0;sub_fold_n++)
        {
            //1.create the filename 
            stringstream ss;
            ss << outfilename;
            ss << "_";
            ss << sub_fold_n;
            ss << ".txt";

             
            fstream fout(ss.str().c_str(),ios::out );

            if( vector_0.size() < vector_1.size() )
            {
                //2. output the fewer class
                for(int i = 0; i < vector_0.size();i++)
                {
                    fout << vector_0.at(i) << " " << endl;
                }
                //3. output the larger class
                if( sub_fold_n != n_vector_1/n_vector_0 -1  )
                {
                    for(int i = sub_fold_n*n_vector_0 ; i < (sub_fold_n+1)*n_vector_0;i++)
                    {
                        fout << vector_1.at(i) << " " << endl;
                    }
                }
                else
                {
                    for(int i = sub_fold_n*n_vector_0 ; i < n_vector_1;i++)
                    {
                        fout << vector_1.at(i) << " " << endl;
                    }
                }
            }
            else
            {
                //2. output the fewer class
                for(int i = 0; i < vector_1.size();i++)
                {
                    fout << vector_1.at(i) << " " << endl;
                }
                //3. output the larger class
                if( sub_fold_n != n_vector_0/n_vector_1 -1  )
                {
                    for(int i = sub_fold_n*n_vector_1 ; i < (sub_fold_n+1)*n_vector_1;i++)
                    {
                        fout << vector_0.at(i) << " " << endl;
                    }
                }
                else
                {
                    for(int i = sub_fold_n*n_vector_1 ; i < n_vector_0;i++)
                    {
                        fout << vector_0.at(i)  << " " << endl;
                    }
                }
            }           
            fout.close();
        }
    }
    else
    {
        cout << "don't need sub-sampling, do relabel only" << endl;
        //1.create the filename 
        stringstream ss;
        ss << outfilename;
        ss << "_";
        ss << 0;
        ss << ".txt";

        //2. output the 0 class 
        fstream fout(ss.str().c_str(),ios::out );
        for(int i = 0; i < vector_0.size();i++)
        {
            fout << vector_0.at(i) << " " << endl;
        }

        //3. output the 1 class
        for(int i = 0 ; i < n_vector_1;i++)
        {
            fout << vector_1.at(i) << " " << endl;
        }
        fout.close();
    }
    
    return subsample_num;
}

vector<double> do_train_test(int n_fold, bool do_CV, string input_filename , string output_modelname,bool SAVE_MODEL)//folding by batch
{
    //read file include all data for train & test => store in vector_0 = class_0 , vector_1 = class_1 ..... format: diagnosis f1 f2 f3 f4.....
    read_alldata_tovector(input_filename);
    //read_alldata_tovector_svm(input_filename);
    cout << "vector_0 size=" << vector_0.size() << " feature num=" << vector_0.at(0).size() << endl;
    cout << "vector_1 size=" << vector_1.size() << " feature num=" << vector_1.at(0).size() << endl;
    cout << "vector_2 size=" << vector_2.size() << endl;
    cout << "vector_3 size=" << vector_3.size() << endl;

    //random pick... skip 

    //accuracy sensitivity specificity precision recall
    vector<double> c1a,c2a,c3a,c4a,c5a,c6a,c7a,c8a,c9a ,c1s,c2s,c3s,c4s,c5s,c6s,c7s,c8s,c9s , c1p,c2p,c3p,c4p,c5p,c6p,c7p,c8p,c9p, c1pre,c2pre,c3pre,c4pre,c5pre,c6pre,c7pre,c8pre,c9pre ; 
    int index = 0;
    for(int i = 0; i < n_fold; i++)//n-folding
    {
        vector<vector<double> > training_data_v,testing_data_v;
        vector<int> training_classifications_v,testing_classifications_v;
        vector<string> test_filename;

        for(int j = 0; j < vector_0.size();j++)
        {
            if(j >= i*(vector_0.size()/n_fold) && j < (i+1)*(vector_0.size()/n_fold))//for test
            {
                testing_data_v.push_back(vector_0.at(j));
                testing_classifications_v.push_back(0);//diagnosis 0 = 0
                test_filename.push_back(vector_filename_0.at(j));
            }
            else//for train
            {
                training_data_v.push_back(vector_0.at(j));
                training_classifications_v.push_back(0);//diagnosis 0 = 0
            }
        }

        for(int j = 0; j < vector_1.size();j++)
        {
            if(j >= i*(vector_1.size()/n_fold) && j < (i+1)*(vector_1.size()/n_fold))//for test
            {
                testing_data_v.push_back(vector_1.at(j));
                testing_classifications_v.push_back(1);//diagnosis 1 = 1
                test_filename.push_back(vector_filename_1.at(j));
            }
            else//for train
            {
                training_data_v.push_back(vector_1.at(j));
                training_classifications_v.push_back(1);//diagnosis 1 = 1
            }
        }

        if(training_data_v.size() != training_classifications_v.size() || testing_data_v.size() != testing_classifications_v.size())
        {
            cout << "error , training_data_v.size() != training_classifications_v.size() || testing_data_v.size() != testing_classifications_v.size()" << endl;

        }   


        ///////to svm_problem
        cout << "to svm_problem" << endl; 
        struct svm_problem prob;        // set by read_problem
        prob.l = training_data_v.size();
        prob.y = new double [prob.l];
        prob.x = new svm_node*[prob.l];

        for(int k =0;k < prob.l;k++)
        {
            prob.y[k] = training_classifications_v.at(k);
            prob.x[k] = new svm_node[training_data_v.at(k).size()+1];//+1 for index
            for(int j = 0;j < training_data_v.at(k).size(); j++)
            {
                prob.x[k][j].index = j+1;
                prob.x[k][j].value = training_data_v.at(k).at(j);
            }
            prob.x[k][ training_data_v.at(k).size()].index = -1;
            prob.x[k][ training_data_v.at(k).size()].value = -1;
        }


        ////////////Training SVM/////////////////////

        ////cross-validation 
        //output to .txt  so we can call grid.py to do cross-validation
        stringstream cg_filename;
        cg_filename << "training_data_for_cg_";
        cg_filename << i;
        cg_filename << ".txt";
        fstream fout_clear(cg_filename.str().c_str(),ios::trunc);//clear content, if file exists
        fstream fout_t(cg_filename.str().c_str(),ios::out);
        cout << "training_data_v.size()=" << training_data_v.size() << " ";
        for(int k = 0;k < training_data_v.size();k++)
        {
            fout_t << training_classifications_v.at(k) << " ";
            for(int j = 0; j < training_data_v.at(k).size();j++)
            {
                fout_t << j+1 << ":" << training_data_v.at(k).at(j) << " ";
            }
            fout_t << endl;
        }
        fout_t.close();

        

        
        if(do_CV)
        {
            stringstream cmd;
            string execPath("tools/grid.py ");
            execPath = LIBSVM_PATH + execPath;
            cmd << execPath;
            cmd << cg_filename.str();

            system(cmd.str().c_str());

            ////read -c -g & set_svm_para
            cout << "cross-validation" << endl;
            double c,g;
            fstream fin_cg;
            fin_cg.open("cg.txt",ios::in);
            fin_cg >> c;
            fin_cg >> g;
            cout << "-c=" << c << " -g=" << g << endl;
            set_svm_para(c,g);
        }
        else
        {
            /*double c,g;
            c = 0;
            g = 0;
            set_svm_para(c,g);*/
        }
        
        


        
        struct svm_model *model = svm_train(&prob,&param);
        if(SAVE_MODEL)
        {
        stringstream modelname;
        modelname << output_modelname;
        modelname << i;
        modelname << ".model";

        if(svm_save_model(modelname.str().c_str(),model) == -1)
            cout << "save model fail " << endl;
        else
            cout << "save model success" << endl;
        }
        /////////////SVM predict////////////////////
        Mat test_sample;
        int correct_class = 0;
        int wrong_class = 0;
        int false_positives [NUMBER_OF_CLASSES] = {0,0};


        ////for probability 
        ////test data vector
        vector<double> prob_vector;
        double* prob_estimates = new double[svm_get_nr_class(model)];
        fstream fout5("fail_case_FP.txt",ios::out);
        fstream fout6("fail_case_FN.txt",ios::out);
        for(int j = 0;j < testing_data_v.size();j++)
        {
            svm_node* node = new svm_node[testing_data_v.at(j).size() + 1 ];
            for(int k = 0; k < testing_data_v.at(j).size();k++)
            {
                node[k].index = k+1;
                node[k].value = testing_data_v.at(j).at(k);
            }
            node[testing_data_v.at(j).size()].index = -1;
            node[testing_data_v.at(j).size()].value = -1;
            svm_predict_probability(model,node,prob_estimates);
            prob_vector.push_back(prob_estimates[1]);
            cout << "prob_estimates = " << prob_estimates[1] << endl;

            if( prob_estimates[1] >= 0.5 )
            {
                if(testing_classifications_v.at(j) == 1)
                    ;//TP
                else
                    fout6 << test_filename.at(j) << endl;//FN
            }
            else
            {
                if(testing_classifications_v.at(j) == 1)
                    fout5 << test_filename.at(j) << endl;//FP
                else
                    ;//TN
            }
        }
        fout5.close();
        fout6.close();

        fstream fout2("FP.txt",ios::out);
        fstream fout3("FN.txt",ios::out);
        fstream fout4("scores.txt",ios::out |ios::app );

        for(float i = 0.1; i < 1;i+=0.1 )//confidence 0.1~0.9
        {
            int hit = 0;
            int TP = 0;
            int TN = 0;
            int FP = 0;
            int FN = 0;
            for(int j = 0;j < prob_vector.size();j++)
            {
                int result;
                if(prob_vector.at(j) >= i)
                    result = 1;
                else
                    result = 0;

                if(result == testing_classifications_v.at(j))
                {
                    hit++;
                    if(result == 1)
                        TP++;
                    else
                        TN++;
                }
                else
                {
                    if(i == 0.5)
                    {
                        if(result == 1)
                        {
                            FP++;
                            fout2 << test_filename.at(j) << endl;
                        }
                        else
                        {
                            FN++;
                            fout3 << test_filename.at(j) << endl;
                        }
                    }
                    else
                    {
                        if(result == 1)
                            FP++;
                        else
                            FN++;
                    }
                }
            }
            switch((int)(i*10))
            {
            case 1:
                c1a.push_back((double)hit/prob_vector.size());
                c1s.push_back((double)TP/(double)(TP+FN));
                c1p.push_back((double)TN/(double)(TN+FP));
                c1pre.push_back( (double)TP/(double)(TP+FP) );
                //recall = sensitivity
                break;
            case 2:
                c2a.push_back((double)hit/prob_vector.size());
                c2s.push_back((double)TP/(double)(TP+FN));
                c2p.push_back((double)TN/(double)(TN+FP));
                c2pre.push_back( (double)TP/(double)(TP+FP) );
                break;
            case 3:
                c3a.push_back((double)hit/prob_vector.size());
                c3s.push_back((double)TP/(double)(TP+FN));
                c3p.push_back((double)TN/(double)(TN+FP));
                c3pre.push_back( (double)TP/(double)(TP+FP) );
                break;
            case 4:
                c4a.push_back((double)hit/prob_vector.size());
                c4s.push_back((double)TP/(double)(TP+FN));
                c4p.push_back((double)TN/(double)(TN+FP));
                c4pre.push_back( (double)TP/(double)(TP+FP) );
                break;
            case 5:
                c5a.push_back((double)hit/prob_vector.size());
                c5s.push_back((double)TP/(double)(TP+FN));
                c5p.push_back((double)TN/(double)(TN+FP));
                c5pre.push_back( (double)TP/(double)(TP+FP) );
                break;
            case 6:
                c6a.push_back((double)hit/prob_vector.size());
                c6s.push_back((double)TP/(double)(TP+FN));
                c6p.push_back((double)TN/(double)(TN+FP));
                c6pre.push_back( (double)TP/(double)(TP+FP) );
                break;
            case 7:
                c7a.push_back((double)hit/prob_vector.size());
                c7s.push_back((double)TP/(double)(TP+FN));
                c7p.push_back((double)TN/(double)(TN+FP));
                c7pre.push_back( (double)TP/(double)(TP+FP) );
                break;
            case 8:
                c8a.push_back((double)hit/prob_vector.size());
                c8s.push_back((double)TP/(double)(TP+FN));
                c8p.push_back((double)TN/(double)(TN+FP));
                c8pre.push_back( (double)TP/(double)(TP+FP) );
                break;
            case 9:
                c9a.push_back((double)hit/prob_vector.size());
                c9s.push_back((double)TP/(double)(TP+FN));
                c9p.push_back((double)TN/(double)(TN+FP));
                c9pre.push_back( (double)TP/(double)(TP+FP) );
                break;
            }
            //cout << TP << " " << TN << " " << FP << " " << FN << endl;
            double precision = (double)TP/(double)(TP+FP);
            double recall =  (double)TP/(double)(TP+FN);  //= sensitivity

            cout << "confidence =" << i << "  accuracy =" << (double)hit/prob_vector.size() 
                << " sensitivity=" << (double)TP/(double)(TP+FN) << " specificity =" << (double)TN/(double)(TN+FP) 
                << " precision=" << precision << " recall=" << recall
                << " F1-score=" <<  2.0*precision*recall/(precision+recall)
                << " TP=" << TP << " TN=" << TN << " FP=" << FP << " FN=" << FN  << endl;
            fout4 << "confidence =" << i << "  accuracy =" << (double)hit/prob_vector.size() 
                << " sensitivity=" << (double)TP/(double)(TP+FN) << " specificity =" << (double)TN/(double)(TN+FP)  
                << " precision=" << precision << " recall=" << recall
                << " F1-score=" <<  2.0*precision*recall/(precision+recall)
                << " TP=" << TP << " TN=" << TN << " FP=" << FP << " FN=" << FN  << endl;
            
        }

        fout2.close();
        fout3.close();
        fout4 << endl;
        fout4.close();


        cout << endl << endl;
    }

    cout << "confidence =" << 0.1 << "  accuracy =" << mean(c1a).val[0]
    << " sensitivity=" << mean(c1s).val[0] << " specificity =" << mean(c1p).val[0] << " precision=" << mean(c1pre).val[0] << " F1-score=" << (double)(2.0*mean(c1pre).val[0]*mean(c1s).val[0])/(double)(mean(c1pre).val[0]+mean(c1s).val[0]) << endl;
    cout << "confidence =" << 0.2 << "  accuracy =" << mean(c2a).val[0]
    << " sensitivity=" << mean(c2s).val[0] << " specificity =" << mean(c2p).val[0] << " precision=" << mean(c2pre).val[0] << " F1-score=" << (double)(2.0*mean(c2pre).val[0]*mean(c2s).val[0])/(double)(mean(c2pre).val[0]+mean(c2s).val[0]) << endl;
    cout << "confidence =" << 0.3 << "  accuracy =" << mean(c3a).val[0]
    << " sensitivity=" << mean(c3s).val[0] << " specificity =" << mean(c3p).val[0] << " precision=" << mean(c3pre).val[0] << " F1-score=" << (double)(2.0*mean(c3pre).val[0]*mean(c3s).val[0])/(double)(mean(c3pre).val[0]+mean(c3s).val[0]) << endl;
    cout << "confidence =" << 0.4 << "  accuracy =" << mean(c4a).val[0]
    << " sensitivity=" << mean(c4s).val[0] << " specificity =" << mean(c4p).val[0] << " precision=" << mean(c4pre).val[0] << " F1-score=" << (double)(2.0*mean(c4pre).val[0]*mean(c4s).val[0])/(double)(mean(c4pre).val[0]+mean(c4s).val[0]) << endl;
    cout << "confidence =" << 0.5 << "  accuracy =" << mean(c5a).val[0]
    << " sensitivity=" << mean(c5s).val[0] << " specificity =" << mean(c5p).val[0] << " precision=" << mean(c5pre).val[0] << " F1-score=" << (double)(2.0*mean(c5pre).val[0]*mean(c5s).val[0])/(double)(mean(c5pre).val[0]+mean(c5s).val[0]) << endl;
    cout << "confidence =" << 0.6 << "  accuracy =" << mean(c6a).val[0]
    << " sensitivity=" << mean(c6s).val[0] << " specificity =" << mean(c6p).val[0] << " precision=" << mean(c6pre).val[0] << " F1-score=" << (double)(2.0*mean(c6pre).val[0]*mean(c6s).val[0])/(double)(mean(c6pre).val[0]+mean(c6s).val[0]) << endl;
    cout << "confidence =" << 0.7 << "  accuracy =" << mean(c7a).val[0]
    << " sensitivity=" << mean(c7s).val[0] << " specificity =" << mean(c7p).val[0] << " precision=" << mean(c7pre).val[0] << " F1-score=" << (double)(2.0*mean(c7pre).val[0]*mean(c7s).val[0])/(double)(mean(c7pre).val[0]+mean(c7s).val[0]) << endl;
    cout << "confidence =" << 0.8 << "  accuracy =" << mean(c8a).val[0]
    << " sensitivity=" << mean(c8s).val[0] << " specificity =" << mean(c8p).val[0] << " precision=" << mean(c8pre).val[0] << " F1-score=" << (double)(2.0*mean(c8pre).val[0]*mean(c8s).val[0])/(double)(mean(c8pre).val[0]+mean(c8s).val[0]) << endl;
    cout << "confidence =" << 0.9 << "  accuracy =" << mean(c9a).val[0]
    << " sensitivity=" << mean(c9s).val[0] << " specificity =" << mean(c9p).val[0] << " precision=" << mean(c9pre).val[0] << " F1-score=" << (double)(2.0*mean(c9pre).val[0]*mean(c9s).val[0])/(double)(mean(c9pre).val[0]+mean(c9s).val[0]) << endl;

    fstream fout("SVM folding result.txt",ios::out |ios::app );
    fout << "confidence =" << 0.1 << "  accuracy =" << mean(c1a).val[0]
    << " sensitivity=" << mean(c1s).val[0] << " specificity =" << mean(c1p).val[0] << " precision=" << mean(c1pre).val[0] << " F1-score=" << (double)(2.0*mean(c1pre).val[0]*mean(c1s).val[0])/(double)(mean(c1pre).val[0]+mean(c1s).val[0]) << endl;
    fout << "confidence =" << 0.2 << "  accuracy =" << mean(c2a).val[0]
    << " sensitivity=" << mean(c2s).val[0] << " specificity =" << mean(c2p).val[0] << " precision=" << mean(c2pre).val[0] << " F1-score=" << (double)(2.0*mean(c2pre).val[0]*mean(c2s).val[0])/(double)(mean(c2pre).val[0]+mean(c2s).val[0]) << endl;
    fout << "confidence =" << 0.3 << "  accuracy =" << mean(c3a).val[0]
    << " sensitivity=" << mean(c3s).val[0] << " specificity =" << mean(c3p).val[0] << " precision=" << mean(c3pre).val[0] << " F1-score=" << (double)(2.0*mean(c3pre).val[0]*mean(c3s).val[0])/(double)(mean(c3pre).val[0]+mean(c3s).val[0]) << endl;
    fout << "confidence =" << 0.4 << "  accuracy =" << mean(c4a).val[0]
    << " sensitivity=" << mean(c4s).val[0] << " specificity =" << mean(c4p).val[0] << " precision=" << mean(c4pre).val[0] << " F1-score=" << (double)(2.0*mean(c4pre).val[0]*mean(c4s).val[0])/(double)(mean(c4pre).val[0]+mean(c4s).val[0]) << endl;
    fout << "confidence =" << 0.5 << "  accuracy =" << mean(c5a).val[0]
    << " sensitivity=" << mean(c5s).val[0] << " specificity =" << mean(c5p).val[0] << " precision=" << mean(c5pre).val[0] << " F1-score=" << (double)(2.0*mean(c5pre).val[0]*mean(c5s).val[0])/(double)(mean(c5pre).val[0]+mean(c5s).val[0]) << endl;
    fout << "confidence =" << 0.6 << "  accuracy =" << mean(c6a).val[0]
    << " sensitivity=" << mean(c6s).val[0] << " specificity =" << mean(c6p).val[0] << " precision=" << mean(c6pre).val[0] << " F1-score=" << (double)(2.0*mean(c6pre).val[0]*mean(c6s).val[0])/(double)(mean(c6pre).val[0]+mean(c6s).val[0]) << endl;
    fout << "confidence =" << 0.7 << "  accuracy =" << mean(c7a).val[0]
    << " sensitivity=" << mean(c7s).val[0] << " specificity =" << mean(c7p).val[0] << " precision=" << mean(c7pre).val[0] << " F1-score=" << (double)(2.0*mean(c7pre).val[0]*mean(c7s).val[0])/(double)(mean(c7pre).val[0]+mean(c7s).val[0]) << endl;
    fout << "confidence =" << 0.8 << "  accuracy =" << mean(c8a).val[0]
    << " sensitivity=" << mean(c8s).val[0] << " specificity =" << mean(c8p).val[0] << " precision=" << mean(c8pre).val[0] << " F1-score=" << (double)(2.0*mean(c8pre).val[0]*mean(c8s).val[0])/(double)(mean(c8pre).val[0]+mean(c8s).val[0]) << endl;
    fout << "confidence =" << 0.9 << "  accuracy =" << mean(c9a).val[0]
    << " sensitivity=" << mean(c9s).val[0] << " specificity =" << mean(c9p).val[0] << " precision=" << mean(c9pre).val[0] << " F1-score=" << (double)(2.0*mean(c9pre).val[0]*mean(c9s).val[0])/(double)(mean(c9pre).val[0]+mean(c9s).val[0]) << endl;
    fout << endl;
    fout.close();
    
    vector<double> result_v;
    result_v.push_back(vector_0.size() + vector_1.size());
    result_v.push_back(mean(c5a).val[0]);
    result_v.push_back(mean(c5s).val[0]);
    result_v.push_back(mean(c5p).val[0]);
    result_v.push_back(mean(c5pre).val[0]);
    
    

    return result_v;

}

vector<double> do_train_test_(int n_fold, bool do_CV, string input_filename , string output_modelname,bool SAVE_MODEL,string type_name, string layer_name)//folding using %
{
	const string type = type_name;//"Alexnet_227";
	const string layer = layer_name;

	clear_vectors();
	//read file include all data for train & test => store in vector_0 = class_0 , vector_1 = class_1 ..... format: diagnosis f1 f2 f3 f4.....
	read_alldata_tovector(input_filename);
	//read_alldata_tovector_svm(input_filename);
	cout << "vector_0 size=" << vector_0.size() << " feature num=" << vector_0.at(0).size() << endl;
   	cout << "vector_1 size=" << vector_1.size() << " feature num=" << vector_1.at(0).size() << endl;
	cout << "vector_2 size=" << vector_2.size() << endl;
	cout << "vector_3 size=" << vector_3.size() << endl;

	//random pick... skip 

	//accuracy sensitivity specificity precision recall
	vector<double> c1a,c2a,c3a,c4a,c5a,c6a,c7a,c8a,c9a ,c1s,c2s,c3s,c4s,c5s,c6s,c7s,c8s,c9s , c1p,c2p,c3p,c4p,c5p,c6p,c7p,c8p,c9p, c1pre,c2pre,c3pre,c4pre,c5pre,c6pre,c7pre,c8pre,c9pre ; 
	int index = 0;
	fstream fout4("scores.txt",ios::out |ios::app);
	//fout4 << "type	layer	round	confidence	Accuracy	Sensitivity	Specificity	Precision	F1-score	TP	TN	FP	FN" << endl;
	fout4 << "type layer round confidence Accuracy Sensitivity Specificity Precision F1-score TP TN FP FN" << endl;
	for(int i = 0; i < n_fold; i++)//n-folding
	{
		vector<vector<double> > training_data_v,testing_data_v;
		vector<int> training_classifications_v,testing_classifications_v;
		vector<string> test_filename;

		for(int j = 0; j < vector_0.size();j++)
		{
			if( j%n_fold == i)//for test
			{
				testing_data_v.push_back(vector_0.at(j));
				testing_classifications_v.push_back(0);
				test_filename.push_back(vector_filename_0.at(j));
			}
			else//for train
			{
				training_data_v.push_back(vector_0.at(j));
				training_classifications_v.push_back(0);
			}
		}

		for(int j = 0; j < vector_1.size();j++)
		{
			if( j%n_fold==i)
			{
				testing_data_v.push_back(vector_1.at(j));
				testing_classifications_v.push_back(1);
				test_filename.push_back(vector_filename_1.at(j));
			}
			else//for train
			{
				training_data_v.push_back(vector_1.at(j));
				training_classifications_v.push_back(1);
			}
		}

		if(training_data_v.size() != training_classifications_v.size() || testing_data_v.size() != testing_classifications_v.size())
		{
			cout << "error , training_data_v.size() != training_classifications_v.size() || testing_data_v.size() != testing_classifications_v.size()" << endl;

		}	


		///////to svm_problem
		cout << "to svm_problem" << endl; 
		struct svm_problem prob;		// set by read_problem
		prob.l = training_data_v.size();
		prob.y = new double [prob.l];
		prob.x = new svm_node*[prob.l];

		for(int k =0;k < prob.l;k++)
		{
			prob.y[k] = training_classifications_v.at(k);
			prob.x[k] = new svm_node[training_data_v.at(k).size()+1];//+1 for index
			for(int j = 0;j < training_data_v.at(k).size(); j++)
			{
				prob.x[k][j].index = j+1;
				prob.x[k][j].value = training_data_v.at(k).at(j);
			}
			prob.x[k][ training_data_v.at(k).size()].index = -1;
			prob.x[k][ training_data_v.at(k).size()].value = -1;
		}
		

		////////////Training SVM/////////////////////

		////cross-validation 
		//output to .txt  so we can call grid.py to do cross-validation
		stringstream cg_filename;
		cg_filename << "training_data_for_cg_";
		cg_filename << i;
		cg_filename << ".txt";
		fstream fout_clear(cg_filename.str().c_str(),ios::trunc);//clear content, if file exists
		fstream fout_t(cg_filename.str().c_str(),ios::out);
		cout << "training_data_v.size()=" << training_data_v.size() << " ";
		for(int k = 0;k < training_data_v.size();k++)
		{
			fout_t << training_classifications_v.at(k) << " ";
			for(int j = 0; j < training_data_v.at(k).size();j++)
			{
				fout_t << j+1 << ":" << training_data_v.at(k).at(j) << " ";
			}
			fout_t << endl;
		}
		fout_t.close();

		

		
		if(do_CV)
		{
			stringstream cmd;
			string execPath("tools/grid.py ");
          		execPath = LIBSVM_PATH + execPath;
            		cmd << execPath;
            		cmd << cg_filename.str();

			system(cmd.str().c_str());

			////read -c -g & set_svm_para
			cout << "cross-validation" << endl;
			double c,g;
			fstream fin_cg;
			fin_cg.open("cg.txt",ios::in);
			fin_cg >> c;
			fin_cg >> g;
			cout << "-c=" << c << " -g=" << g << endl;
			set_svm_para(c,g);
		}
		else
		{
			/*double c,g;
			c = 0;
			g = 0;
			set_svm_para(c,g);*/
		}
		
		


		stringstream modelname;
		modelname << output_modelname;
		modelname << i;
		modelname << ".model";

		struct svm_model *model = svm_train(&prob,&param);
		if(SAVE_MODEL)
   		{
   		     stringstream modelname;
        	     modelname << output_modelname;
        	     modelname << i;
                     modelname << ".model";

		        if(svm_save_model(modelname.str().c_str(),model) == -1)
            		cout << "save model fail " << endl;
        		else
            		cout << "save model success" << endl;
        	}
		
		/////////////SVM predict////////////////////
		Mat test_sample;
		int correct_class = 0;
		int wrong_class = 0;
		int false_positives [NUMBER_OF_CLASSES] = {0,0};


		////for probability 
		////test data vector
		vector<double> prob_vector;
		double* prob_estimates = new double[svm_get_nr_class(model)];
		fstream fout5("fail_case_FP.txt",ios::out);
		fstream fout6("fail_case_FN.txt",ios::out);
		for(int j = 0;j < testing_data_v.size();j++)
		{
			svm_node* node = new svm_node[testing_data_v.at(j).size() + 1 ];
			for(int k = 0; k < testing_data_v.at(j).size();k++)
			{
				node[k].index = k+1;
				node[k].value = testing_data_v.at(j).at(k);
			}
			node[testing_data_v.at(j).size()].index = -1;
			node[testing_data_v.at(j).size()].value = -1;
			svm_predict_probability(model,node,prob_estimates);
			prob_vector.push_back(prob_estimates[1]);
			cout << "prob_estimates = " << prob_estimates[1] << endl;

			if( prob_estimates[1] >= 0.5 )
			{
				if(testing_classifications_v.at(j) == 1)
					;//TP
				else
					fout6 << test_filename.at(j) << endl;//FN
			}
			else
			{
				if(testing_classifications_v.at(j) == 1)
					fout5 << test_filename.at(j) << endl;//FP
				else
					;//TN
			}
			delete[] node;
		}
		fout5.close();
		fout6.close();

		fstream fout2("FP.txt",ios::out |ios::app);
		fstream fout3("FN.txt",ios::out |ios::app);
		
		int round = i; // for record round = i outside ...
		for(float i = 0.1; i < 1;i+=0.1 )//confidence 0.1~0.9
		{
			int hit = 0;
			int TP = 0;
			int TN = 0;
			int FP = 0;
			int FN = 0;
			for(int j = 0;j < prob_vector.size();j++)
			{
				int result;
				if(prob_vector.at(j) >= i)
					result = 1;
				else
					result = 0;

				if(result == testing_classifications_v.at(j))
				{
					hit++;
					if(result == 1)
						TP++;
					else
						TN++;
				}
				else
				{
					if(i == 0.5)
					{
						if(result == 1)
						{
							FP++;
							fout2 << test_filename.at(j) << endl;
						}
						else
						{
							FN++;
							fout3 << test_filename.at(j) << endl;
						}
					}
					else
					{
						if(result == 1)
							FP++;
						else
							FN++;
					}
				}
			}
			double accuracy = (double)hit/prob_vector.size();
			if(prob_vector.size() == 0)
				accuracy = -100;

			double precision = (double)TP/(double)(TP+FP);
			if((TP+FP) == 0)
				precision = 0;
			double SE =  (double)TP/(double)(TP+FN);  //= recall
			if((TP+FN) == 0)
				SE = 0;
			double SP = (double)TN/(double)(TN+FP);
			if((TN+FP) == 0)
				SP = 0;
			double F_1_score =  2.0*precision*SE/(precision+SE);
			if((precision+SE) == 0)
				F_1_score = 0;

			switch((int)(i*10))
			{
			case 1:
				c1a.push_back(accuracy);
				c1s.push_back( SE );
				c1p.push_back(SP);
				c1pre.push_back( precision );
				//recall = sensitivity
				break;
			case 2:
				c2a.push_back(accuracy);
				c2s.push_back(SE);
				c2p.push_back(SP);
				c2pre.push_back( precision );
				break;
			case 3:
				c3a.push_back(accuracy);
				c3s.push_back(SE);
				c3p.push_back(SP);
				c3pre.push_back( precision );
				break;
			case 4:
				c4a.push_back(accuracy);
				c4s.push_back(SE);
				c4p.push_back(SP);
				c4pre.push_back( (double)TP/(double)(TP+FP) );
				break;
			case 5:
				c5a.push_back(accuracy);
				c5s.push_back(SE);
				c5p.push_back(SP);
				c5pre.push_back( precision);
				break;
			case 6:
				c6a.push_back(accuracy);
				c6s.push_back(SE);
				c6p.push_back(SP);
				c6pre.push_back( precision );
				break;
			case 7:
				c7a.push_back(accuracy);
				c7s.push_back(SE);
				c7p.push_back(SP);
				c7pre.push_back( precision );
				break;
			case 8:
				c8a.push_back(accuracy);
				c8s.push_back(SE);
				c8p.push_back(SP);
				c8pre.push_back( precision );
				break;
			case 9:
				c9a.push_back(accuracy);
				c9s.push_back(SE);
				c9p.push_back(SP);
				c9pre.push_back( precision );
				break;
			}
			//cout << TP << " " << TN << " " << FP << " " << FN << endl;
			

			

			cout << type << " " 
				<< layer << " "
				<< round+1 << " " //round
				<< i << " " //confidence
				<<  (double)hit/prob_vector.size() << " " //Accuracy
				<<  SE << " " //Sensitivity
				<<  SP << " " //Specificity
				<<  precision << " "                //precision
				<<  F_1_score << " " //F1-score
				<<  TP << " " 
				<<  TN << " " 
				<<  FP << " " 
				<<  FN 
				<< endl;

			//"type	layer	round	confidence	Accuracy	Sensitivity	Specificity	Precision	F1-score	TP	TN	FP	FN"
			fout4 << type << " " 
				<< layer << " "
				<< round+1 << " " //round
				<< i << " " //confidence
				<<  (double)hit/prob_vector.size() << " " //Accuracy
				<<  SE << " " //Sensitivity
				<<  SP << " " //Specificity
				<<  precision << " "                //precision
				<<  F_1_score << " " //F1-score
				<<  TP << " " 
				<<  TN << " " 
				<<  FP << " " 
				<<  FN 
				<< endl;
		}

		fout2.close();
		fout3.close();

		cout << endl << endl;

		//realease
		svm_free_and_destroy_model(&model);
		delete[] prob_estimates;
		//struct svm_problem prob;		// set by read_problem
		/*for(int x = 0;x < prob.l; x++)
		{
			delete[] prob[x];
		}*/
		for(int k =0;k < prob.l;k++)
		{
			delete[] prob.x[k];
		}
		delete[] prob.y;
		delete[] prob.x;

	}
	fout4.close();

	


	cout << type << " " << layer << " " << 0.1 << " " << mean(c1a).val[0] << " " <<  mean(c1s).val[0] << " " << mean(c1p).val[0] << " " << mean(c1pre).val[0] << " " << (double)(2.0*mean(c1pre).val[0]*mean(c1s).val[0])/(double)(mean(c1pre).val[0]+mean(c1s).val[0]) << endl
		<< type << " " << layer << " " << 0.2 << " " << mean(c2a).val[0] << " " <<  mean(c2s).val[0] << " " << mean(c2p).val[0] << " " << mean(c2pre).val[0] << " " << (double)(2.0*mean(c2pre).val[0]*mean(c2s).val[0])/(double)(mean(c2pre).val[0]+mean(c2s).val[0]) << endl
		<< type << " " << layer << " " << 0.3 << " " << mean(c3a).val[0] << " " <<  mean(c3s).val[0] << " " << mean(c3p).val[0] << " " << mean(c3pre).val[0] << " " << (double)(2.0*mean(c3pre).val[0]*mean(c3s).val[0])/(double)(mean(c3pre).val[0]+mean(c3s).val[0]) << endl
		<< type << " " << layer << " " << 0.4 << " " << mean(c4a).val[0] << " " <<  mean(c4s).val[0] << " " << mean(c4p).val[0] << " " << mean(c4pre).val[0] << " " << (double)(2.0*mean(c4pre).val[0]*mean(c4s).val[0])/(double)(mean(c4pre).val[0]+mean(c4s).val[0]) << endl
		<< type << " " << layer << " " << 0.5 << " " << mean(c5a).val[0] << " " <<  mean(c5s).val[0] << " " << mean(c5p).val[0] << " " << mean(c5pre).val[0] << " " << (double)(2.0*mean(c5pre).val[0]*mean(c5s).val[0])/(double)(mean(c5pre).val[0]+mean(c5s).val[0]) << endl
		<< type << " " << layer << " " << 0.6 << " " << mean(c6a).val[0] << " " <<  mean(c6s).val[0] << " " << mean(c6p).val[0] << " " << mean(c6pre).val[0] << " " << (double)(2.0*mean(c6pre).val[0]*mean(c6s).val[0])/(double)(mean(c6pre).val[0]+mean(c6s).val[0]) << endl
		<< type << " " << layer << " " << 0.7 << " " << mean(c7a).val[0] << " " <<  mean(c7s).val[0] << " " << mean(c7p).val[0] << " " << mean(c7pre).val[0] << " " << (double)(2.0*mean(c7pre).val[0]*mean(c7s).val[0])/(double)(mean(c7pre).val[0]+mean(c7s).val[0]) << endl
		<< type << " " << layer << " " << 0.8 << " " << mean(c8a).val[0] << " " <<  mean(c8s).val[0] << " " << mean(c8p).val[0] << " " << mean(c8pre).val[0] << " " << (double)(2.0*mean(c8pre).val[0]*mean(c8s).val[0])/(double)(mean(c8pre).val[0]+mean(c8s).val[0]) << endl
		<< type << " " << layer << " " << 0.9 << " " << mean(c9a).val[0] << " " <<  mean(c9s).val[0] << " " << mean(c9p).val[0] << " " << mean(c9pre).val[0] << " " << (double)(2.0*mean(c9pre).val[0]*mean(c9s).val[0])/(double)(mean(c9pre).val[0]+mean(c9s).val[0]) << endl;
		


	fstream fout("SVM folding result.txt",ios::out |ios::app);
	fout << "type layer confidence Accuracy Sensitivity Specificity Precision F1-score " << endl;
	fout << type << " " << layer << " " << 0.1 << " " << mean(c1a).val[0] << " " <<  mean(c1s).val[0] << " " << mean(c1p).val[0] << " " << mean(c1pre).val[0] << " " << (double)(2.0*mean(c1pre).val[0]*mean(c1s).val[0])/(double)(mean(c1pre).val[0]+mean(c1s).val[0]) << endl
		<< type << " " << layer << " " << 0.2 << " " << mean(c2a).val[0] << " " <<  mean(c2s).val[0] << " " << mean(c2p).val[0] << " " << mean(c2pre).val[0] << " " << (double)(2.0*mean(c2pre).val[0]*mean(c2s).val[0])/(double)(mean(c2pre).val[0]+mean(c2s).val[0]) << endl
		<< type << " " << layer << " " << 0.3 << " " << mean(c3a).val[0] << " " <<  mean(c3s).val[0] << " " << mean(c3p).val[0] << " " << mean(c3pre).val[0] << " " << (double)(2.0*mean(c3pre).val[0]*mean(c3s).val[0])/(double)(mean(c3pre).val[0]+mean(c3s).val[0]) << endl
		<< type << " " << layer << " " << 0.4 << " " << mean(c4a).val[0] << " " <<  mean(c4s).val[0] << " " << mean(c4p).val[0] << " " << mean(c4pre).val[0] << " " << (double)(2.0*mean(c4pre).val[0]*mean(c4s).val[0])/(double)(mean(c4pre).val[0]+mean(c4s).val[0]) << endl
		<< type << " " << layer << " " << 0.5 << " " << mean(c5a).val[0] << " " <<  mean(c5s).val[0] << " " << mean(c5p).val[0] << " " << mean(c5pre).val[0] << " " << (double)(2.0*mean(c5pre).val[0]*mean(c5s).val[0])/(double)(mean(c5pre).val[0]+mean(c5s).val[0]) << endl
		<< type << " " << layer << " " << 0.6 << " " << mean(c6a).val[0] << " " <<  mean(c6s).val[0] << " " << mean(c6p).val[0] << " " << mean(c6pre).val[0] << " " << (double)(2.0*mean(c6pre).val[0]*mean(c6s).val[0])/(double)(mean(c6pre).val[0]+mean(c6s).val[0]) << endl
		<< type << " " << layer << " " << 0.7 << " " << mean(c7a).val[0] << " " <<  mean(c7s).val[0] << " " << mean(c7p).val[0] << " " << mean(c7pre).val[0] << " " << (double)(2.0*mean(c7pre).val[0]*mean(c7s).val[0])/(double)(mean(c7pre).val[0]+mean(c7s).val[0]) << endl
		<< type << " " << layer << " " << 0.8 << " " << mean(c8a).val[0] << " " <<  mean(c8s).val[0] << " " << mean(c8p).val[0] << " " << mean(c8pre).val[0] << " " << (double)(2.0*mean(c8pre).val[0]*mean(c8s).val[0])/(double)(mean(c8pre).val[0]+mean(c8s).val[0]) << endl
		<< type << " " << layer << " " << 0.9 << " " << mean(c9a).val[0] << " " <<  mean(c9s).val[0] << " " << mean(c9p).val[0] << " " << mean(c9pre).val[0] << " " << (double)(2.0*mean(c9pre).val[0]*mean(c9s).val[0])/(double)(mean(c9pre).val[0]+mean(c9s).val[0]) << endl;
	fout << endl;
	Scalar mean_value;
	Scalar std;
	meanStdDev(c5a,mean_value,std);
	cout << "std of accuracy=" << std.val[0] << endl;
	fout << "std of accuracy=" << std.val[0] << endl;
	meanStdDev(c5s,mean_value,std);
	cout << "std of sensitiviy=" << std.val[0] << endl;
	fout << "std of sensitiviy=" << std.val[0] << endl;
	meanStdDev(c5p,mean_value,std);
	cout << "std of specificity=" << std.val[0] << endl;
	fout << "std of specificity=" << std.val[0] << endl;
	meanStdDev(c5pre,mean_value,std);
	cout << "std of pre=" << std.val[0] << endl;
	fout << "std of pre=" << std.val[0] << endl;
	
	fout << endl;
	double a_mean = mean(c5a).val[0];
	double s_mean = mean(c5s).val[0];
	double p_mean = mean(c5p).val[0];
	double pre_mean = mean(c5pre).val[0];
	
	double a_sum = 0;
	double s_sum = 0;
	double p_sum = 0;
	double pre_sum = 0;
	for(int x = 0;x < c5a.size();x++)
	{
		a_sum+= abs(a_mean-c5a.at(x));
		s_sum+= abs(s_mean-c5s.at(x));
		p_sum+= abs(p_mean-c5p.at(x));
		pre_sum+= abs(pre_mean-c5pre.at(x));
	}
	cout << "avg variance of accuaracy =" << a_sum/c5a.size() << endl
	 << "avg variance of sensitivity =" << s_sum/c5a.size() << endl
	 << "avg variance of specificity =" << p_sum/c5a.size() << endl
	 << "avg variance of precision =" << pre_sum/c5a.size() << endl;

	fout << "avg variance of accuaracy =" << a_sum/c5a.size() << endl 
	    << "avg variance of sensitivity =" << s_sum/c5a.size() << endl
	    << "avg variance of specificity =" << p_sum/c5a.size() << endl
	    << "avg variance of precision =" << pre_sum/c5a.size() << endl;

	fout.close();

	vector<double> result_v;
	result_v.push_back(vector_0.size() + vector_1.size());
	result_v.push_back(mean(c5a).val[0]);
	result_v.push_back(mean(c5s).val[0]);
	result_v.push_back(mean(c5p).val[0]);
	result_v.push_back(mean(c5pre).val[0]);
	
	

	return result_v;

}
//***input: (1)K-folidng; (2)Do_subsample(0=false, 1=yes); (3) save_model(0=flase 1=true) 
//          (4)feature_filename.txt(filename 1:f1 2:f2 ...); (5)label.txt; (6):typename (7):layername
int main(int argc, char** argv)
{   
    if(argc < 5)
    {
        cout << "error, plz input (1)K-folidng; (2)Do_subsample(0=false, 1=true); (3) save_model(0=flase 1=true)       (4)feature_filename.txt(filename 1:f1 2:f2 ...); (5)label.txt (6):typename (7):layername" << endl;
        return 0;
    }

    //cout << "---------------------SVM " << argv[1] << "-folding---------------------" << endl; 
    int k_folding = atoi(argv[1]);
    bool do_CV = true;
    bool do_subsampling = atoi(argv[2]);
    bool SAVE_MODEL = atoi(argv[3]); 
    string type_name = argv[6];
    string layer_name = argv[7]; 


    ////convert filname to label and save to "Relabel.txt"   
    Relabel(argv[4],argv[5]);  //"OM_Caffe.txt"
    cout << "relabeling over" << endl;

    ////// scale 
    vector<string> input; 
    vector<string> output;
    input.push_back("Relabeled.txt");
    output.push_back("Relabeled_scaled.txt");
    SVM_scale(input,-1.0, 1.0,output);

    cout << "SVM_scale over" << endl;
    ////subsample & & relabel to 2 classes 

    if(do_subsampling) 
    {
        int subsample_num = sub_sample("Relabeled_scaled.txt","subsample"); // subsample_para = subsample_num , vector_0.size(), vector_1.size() 
        vector<double> scores_v;// scores_v = size(vector_0 + vector_1) , accuracy , SE , SP , precision
        double total_size = 0;
        for( int i = 0; i <  subsample_num ; i++)
        {
            stringstream input_filename; // "subsample_0.txt"; 
            input_filename << "subsample_";
            input_filename << i;
            input_filename << ".txt";

            stringstream output_modelname;  //"subsample_0_";    
            output_modelname << "subsample_";
            output_modelname << i;
            output_modelname << "_";

            if(scores_v.size() == 0)
            {
		clear_vectors();
                scores_v = do_train_test_(k_folding,do_CV, input_filename.str(), output_modelname.str(),SAVE_MODEL,type_name,layer_name);
				total_size = scores_v.at(0);
				for(int j = 1; j < scores_v.size(); j++)
				{
					scores_v.at(j) = scores_v.at(0)*scores_v.at(j);
				}
            }
            else
            {
                clear_vectors();
                vector<double> tmp = do_train_test_(k_folding,do_CV,input_filename.str(), output_modelname.str(),SAVE_MODEL,type_name,layer_name);
				total_size += tmp.at(0);
				for(int j = 1; j < tmp.size(); j++)
				{
					scores_v.at(j) += tmp.at(0)*tmp.at(j);
				}
            }
        }

        fstream fout("SVM folding result.txt",ios::app);
		fout << endl;
		fout << "Avg:  Accuracy =" << scores_v.at(1)/total_size << " Sensitivity=" << scores_v.at(2)/total_size << " Specificity =" << scores_v.at(3)/total_size << " Precision=" << scores_v.at(4)/total_size 
			<< " F1-score=" << (double)(2.0*scores_v.at(2)*scores_v.at(4) )/(double)( scores_v.at(2) + scores_v.at(4) )/total_size << endl;
		fout.close();
		
		cout << endl; 
		cout << endl;
		cout << "Avg:  Accuracy =" << scores_v.at(1)/total_size << " Sensitivity=" << scores_v.at(2)/total_size << " Specificity =" << scores_v.at(3)/total_size << " Precision=" << scores_v.at(4)/total_size 
			<< " F1-score=" << (double)(2.0*scores_v.at(2)*scores_v.at(4) )/(double)( scores_v.at(2) + scores_v.at(4) )/total_size << endl;
    } 
    else
    {
        clear_vectors();
        string input_filename = "Relabeled_scaled.txt";
        string output_modelname = "non_subsample_";
        do_train_test_(k_folding,do_CV, input_filename, output_modelname,SAVE_MODEL,type_name,layer_name);
    }

    system("pause");
    return 0;
}













