#include<fstream>
#include<iostream>
#include<vector>
#include<algorithm>
 
using namespace std;
 
#define DEMENSION 5  //数据维度
 
//样本结构体
struct record{
	double x[DEMENSION];
	int y;
};
 
//读取文件数据
void getData(fstream &datafile,vector<record> &dataset){
	while(!datafile.eof()){
		record curRecord;
		curRecord.x[0] = 1;
		int i;
		for(i=1 ; i<DEMENSION ; i++)datafile>>curRecord.x[i];
		datafile>>curRecord.y;
		dataset.push_back(curRecord);
	}
	datafile.close();
}
 
//计算sign值
int sign(double x){
	if(x <= 0)return -1;
	else return 1;
}
 
//计算两个向量内积，判断是否需要修正
double multiply(double *v1, double *v2){
	int i;
	double temp = 0.0;
	for(i = 0; i < DEMENSION; i++)temp += v1[i] * v2[i];
	return temp;
}
 
//函数重载，计算向量v与整数num的积，用于计算y*x（y为+1或-1，x为向量）
void multiply(double *result,double *v,int num){
	int i;
	for(i = 0; i < DEMENSION; i++)result[i] =  num * v[i];
}
 
//计算两向量的和放入result中，用于计算w(i+1)=w(i)+y*x
void add(double *result,double *v1,double *v2){
	int i;
	for(i = 0; i < DEMENSION; i++)result[i] = v1[i] + v2[i];
}
 
//计算错误率
double getErrorRate(double *weight,vector<record> dataset){
	int n = dataset.size();
	double errorRate= 0.0;
	int i;
	for(i=0;i<n;i++)
		if(sign(multiply(weight,dataset[i].x)) != dataset[i].y)errorRate++;
	return errorRate/n;
}
 
//口袋PLA算法
void pocketPLA(double *pocketWeights,double *weights,vector<record> trainingSet,int iteration){
	int index = 0;
	int iter= 1;
	int n = trainingSet.size();
	while(iter < iteration){
		if(sign(multiply(trainingSet[index].x,weights)) != trainingSet[index].y){
			double temp[DEMENSION];
			multiply(temp,trainingSet[index].x,trainingSet[index].y);
			int i;
			for(i=0;i<DEMENSION;i++)weights[i] += temp[i];
	    	if(getErrorRate(weights,trainingSet) < getErrorRate(pocketWeights,trainingSet)){
				int j;
				for(j = 0;j<DEMENSION;j++)pocketWeights[j] = weights[j]; 
			}
			iter++;
		}
		if(index == n-1)index = 0;
		else index++;
	}
}
 
int main(void){
	vector<record> trainingSet;
	vector<record> testSet;
	fstream datafile1("hw1_18_train.dat.txt");
	fstream datafile2("hw1_18_test.dat.txt");
	if(datafile1.is_open()&&datafile2.is_open()){
		getData(datafile1,trainingSet);
		getData(datafile2,testSet);
	}
	else{
		cout<<"can not open file!"<<endl;
		exit(1);
	}
	double weights[DEMENSION],pocketWeights[DEMENSION];
	
	double ave_error = 0.0 ;
	int j;
	for(j = 0; j < 2000; j++ ){
 
		random_shuffle(trainingSet.begin(), trainingSet.end());
 
		int i;
		for(i=0;i<DEMENSION;i++){  //注意，这里需要初始化！！！不初始化值会乱码，程序出错！！！
		    weights[i]=0.0;
		    pocketWeights[i]=0.0;
		}
		pocketPLA(pocketWeights,weights,trainingSet,100);
	    double trainingError = getErrorRate(pocketWeights,trainingSet);
   	    double testError = getErrorRate(pocketWeights,testSet);
		ave_error += testError;
		cout<<j<<"---"<<"training error:"<<trainingError<<" test error:"<<testError<<endl;
 
	}
	cout<<"average error rate:"<<ave_error/2000<<endl;
}