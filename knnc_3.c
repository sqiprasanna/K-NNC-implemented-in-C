#include<stdio.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>

int train_set[6670][193],test_set[3333][193];
int dist1[2223][4446];
int a1[2223][195],a2[2223][195],a3[2223][195];

struct distance_index{
    float distance;
    int index;
};
struct distance_index first_val_set[2223][4446];
struct distance_index second_val_set[2223][4446];
struct distance_index third_val_set[2223][4446];

struct distance_index test1[3333][6669];
float first_val_set_errors[25];
float second_val_set_errors[25];
float third_val_set_errors[25];
float final_error[25];
int min_ele;
int count_test[3333][10];

void swap(struct distance_index* a, struct distance_index* b)
{
    struct distance_index t = *a;
    *a = *b;
    *b = t;
}

int partition (struct distance_index arr[], int low, int high)
{
    struct distance_index pivot = arr[high];    // pivot
    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high- 1; j++)
    {
        if (arr[j].distance <= pivot.distance)
        {
            i++;    // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(struct distance_index arr[], int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(arr, low, high);

        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
/*
void merge(struct distance_index arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
    struct distance_index L[n1], R[n2];
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
 
    /* Merge the temp arrays back into arr[l..r]
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i].distance <= R[j].distance)
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(struct distance_index arr[], int l, int r)
{
    if (l < r)
    {
        int m = l+(r-l)/2;
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);

        merge(arr, l, m, r);
    }
}
*/
float distance_cal(int val1,int val2){	
	float sum;
    sum =0;
    int i,each_row_ts[192],each_row_vs[192];
    for(i=0;i<192;i++){
        each_row_vs[i] = train_set[val1][i];
        each_row_ts[i] = train_set[val2][i];
         sum = sum + (each_row_ts[i]-each_row_vs[i])*(each_row_ts[i]-each_row_vs[i]);
    }	
    return sqrt(sum);
}


void cal_dist_error(int fold){
	
	int i,j;
	if(fold == 1){
        float dist;
		for(i=0;i<2223;i++){
			for(j=2223;j<6669;j++){
			// printf("2");		
                dist = distance_cal(i,j);
              //  printf("3");
                first_val_set[i][j-2223].distance = dist;
                first_val_set[i][j-2223].index = j;
		    }
	    }
    }
    else if(fold == 2){
    float dist1,dist;
        for(int i=2223;i<4446;i++){
            for(int j=0;j<2223;j++){
                dist = distance_cal(i,j);
               // printf("4");
                dist1 =distance_cal(i,j+4446);
               // printf("6");
                second_val_set[i-2223][j].distance = dist;
                second_val_set[i-2223][j].index =j;
                second_val_set[i-2223][j+2223].distance = dist1;
                second_val_set[i-2223][j+2223].index = j+4446;
            }
        }    
    }
    else if(fold ==3){
        float dist;
        for(int i=6668;i>=4446;i--){
            for(int j=0;j<4446;j++){
                dist = distance_cal(i,j);
                third_val_set[6668-i][j].distance = dist;
                third_val_set[6668-i][j].index = j;
            }
        }
    }

} 
void  validation_error(int fold){
    float total_error;
    int label,i,j,k,index_fold;
    for(k=1;k<=25;k++){
        if(fold == 1){
            total_error = 0;
            //printf("valset");
            for(i=0;i<2223;i++){
                label = train_set[i][192];
                for(j=0;j<k;j++){
                    index_fold = first_val_set[i][j].index;
                    if(label != train_set[index_fold][192]){
                        total_error++;
                        
                    }
                } 
            }
         //  printf("%f",total_error);
         first_val_set_errors[k-1] = (float)total_error/(2223*k) ;
        }
    
        else if(fold == 2){
             total_error = 0;
            for(i=2223;i<4446;i++){
                label = train_set[i][192];
                for(j=0;j<k;j++){
                    index_fold = second_val_set[i-2223][j].index;
                    if(label != train_set[index_fold][192]){
                        total_error++;
                    }
                } 
            }
         second_val_set_errors[k-1] = (float)total_error/(2223*k) ;
        }
        else if(fold == 3){
             total_error = 0;
            for(i=6668;i>=4446;i--){
                label = train_set[i][192];
                for(j=0;j<k;j++){
                    index_fold = third_val_set[6668-i][j].index;
                    if(label != train_set[index_fold][192]){
                        total_error++;
                    }
                } 
            }
          third_val_set_errors[k-1] = (float)total_error/(2223*k) ;
        }
    }
}
int find_avg_of_errors(){
    int i;
    float error;
	printf("\nAverage for Each K value:- \n");
    for(i=0;i<25;i++){
      error=0;
      error = (first_val_set_errors[i]+second_val_set_errors[i]+third_val_set_errors[i])/3;
      final_error[i] = error;
      
      printf("%d -%f \n",i,final_error[i]);
    }
    printf("\n");
}
int find_min_element_in_error_arr(){
    int max = 10000000;
    int i,index;
    for(i=0;i<25;i++){
        if(max > final_error[i]){
            max = final_error[i];
            index = i;
        }
    }
    return index;
}


int train(){
	int i,j,k,l;
	   printf("Training the data...... \n");
	   cal_dist_error(1);
        for(int i=0;i<2223;i++){
		    quickSort(first_val_set[i],0,4445);
		    //printf("dsa");
        }
        printf("\n1st fold validation completed Error:- \n");
	   validation_error(1);
	   for(i=0;i<25;i++){
        	printf(" k = %d Error :-%f \n",i ,first_val_set_errors[i]);
        }
        cal_dist_error(2);
        
        for(int i=0;i<2223;i++){
		    quickSort(second_val_set[i],0,4445);
        }
        validation_error(2);
        printf("\n2nd fold validation completed Error:- \n");
        for(i=0;i<25;i++){
        	printf(" k = %d Error :-%f \n",i,second_val_set_errors[i]);
        }
        cal_dist_error(3);
        for(int i=0;i<2223;i++){
		    quickSort(third_val_set[i],0,4445);
        }
        validation_error(3);
	   printf("\n3nd fold validation completed Error:- \n");
        for(i=0;i<25;i++){
        	printf(" k = %d Error :-%f \n",i,third_val_set_errors[i]);
        }	
        find_avg_of_errors();
       return min_ele =  find_min_element_in_error_arr() + 1;
}

int maxi(int nu){
    int min = count_test[nu][0];
    int i,index;
    for( i=0;i<10;i++){
        if(min<count_test[nu][i]){
        min = count_test[nu][i];
        index = i;
        }
  }
  return index;
}

float cal_distance_test(int val1,int val2){
    float sum;
    int i,each_row_ts[192],each_row_vs[192];
    for(i=0;i<192;i++){
        each_row_vs[i] = test_set[val1][i];
        each_row_ts[i] = test_set[val2][i];
    }	
    sum =0;
    for(i = 0;i<192;i++){
        sum = sum + pow((each_row_ts[i]-each_row_vs[i]),2);
    }
    return sqrt(sum);
}

void test(){
    int i,j;
    float distance;
    for(i=0;i<3333;i++){
        for(j=0;j<6669;j++){
            distance = cal_distance_test(i,j);
            test1[i][j].distance = distance;
            test1[i][j].index = j;
        }
    }
    for(i=0;i<3333;i++){
        quickSort(test1[i],0,6668);
    }
    for(i=0;i<3333;i++){
        for(j=0;j<10;j++){
            test_set[i][j] = 0;
        }
    } 
}

void test_error_check(){
    int i,j,label,count =0 ;
    int label_obt,train_label;
    for(i=0;i<3333;i++){
        label = test_set[i][192];
        for(j=0;j<min_ele;j++){
            label_obt = test1[i][j].index;
            train_label = train_set[label_obt][192];
            if(label == train_label){
                 count_test[i][label_obt] +=  1;    
            }
        }
    }
    for(i=0;i<3333;i++){
        int max = maxi(i);
        if(test_set[i][192] == max){
       // printf("%d    ",i);
            count++;
        }
    }
    printf("count:- %d \n",count);
    double accuracy = ((3333-count)/3333.0)*100;
    printf("Accuracy :-%lf \n",accuracy);
}




int main(){
    int i, j, n,k;
    
  
     FILE* f = fopen("pp_tra.txt", "r");
    
    while(getc(f) != EOF){
        for(i=0;i<6669;i++){
            for(j=0;j<193;j++){
               
                fscanf(f,"%d",&train_set[i][j]);
          
            }
        }
    }
   

    FILE* f1 = fopen("pp_tes.txt", "r");
     //printf("dsaf");
    while(getc(f1) != EOF){
        for(i=0;i<3333;i++){
            for(j=0;j<193;j++){
                fscanf(f1,"%d ",&test_set[i][j]);
            }
        }
    }
    
    k = train();
    printf("K value:- %d \n", k);
    test();
    test_error_check();
     fclose(f1);
return 0;
}
