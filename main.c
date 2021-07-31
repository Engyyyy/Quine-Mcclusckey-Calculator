#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
int var;
int minterms;
int* terms;
int covered_minterms = 0; int* terms_1; int* terms_2; int* cost;
int size_pi = 0, size_counter_pi = 0; int size_terms_pi = 0;
int** index1; int** minimize; int** minimize2; int** index2; int** index3;
int* counter3; int* counter_mini2;
int* counter; int* counter2; int* prime_implicants; int* counter_pi; int* terms_pi; int*counter_terms_pi;
int* EPI; int* counter_epi; int size_epi = 0; int size_counter_epi = 0;
int* binary; int bits = 0;
char** str; int* counter_str; char* petrick_arr; int counter_petrick_arr =0;

//char test[10][100] = {"ab","cd","ef",""};


int numberOfOnes(int n){
    binary = (int*) calloc(var, sizeof(int));
    bits = 0;
    int ones = 0;
    while(n > 0){
        if(n % 2 == 1) ones++;
        binary[bits++] = n % 2;
        n /= 2;
    }
    return ones;
}
void filteration(int tables){
    int step = tables; int count = 0; int s; int rowl = -1; int rowx;
    for(int i = 0; i < var+2-tables; i++){
        s = counter2[i]; rowl = -1;
        for(int l = 0; l < s; l += tables){
            rowl++; rowx = rowl-1;
            if(minimize[i][l] == -2) continue;
            for(int x = step+l; x < s; x += tables){
                rowx++;
                if(minimize[i][l] != minimize[i][x]) continue;
                count = 0;
                for(int j = 1; j < tables; j++){
                    for(int k = 1; k < tables; k++){
                        if(minimize[i][l+j] == minimize[i][x+k]){
                            count++;
                            break;
                        }
                    }
                }
                if(count == tables-1){
                    for(int k = 0; k < tables; k++){
                        minimize[i][x+k] = -2;
                    }
                    for(int k = rowx*pow(2, tables-1); k < pow(2, tables-1) * (rowx+1); k++){
                        minimize2[i][k] = -2;
                    }
                }
            }
        }
    }
}

void alternate(int tables){
    index1 = (int**)realloc(index1, (var+2-tables)*sizeof(int*));
    index2 = (int**)realloc(index2, (var+2-tables)*sizeof(int*));
    index3 = (int**)realloc(index3, (var+2-tables)*sizeof(int*));
    counter = (int*) realloc(counter, (var+2-tables)*sizeof(int));
            for(int i = 0; i < var+2-tables; i++){
                counter[i] = 0;
                counter3[i] = 0;
                index1[i] = (int*)malloc(minterms*tables * sizeof(int));
                index2[i] = (int*)malloc(minterms*tables * sizeof(int));
                index3[i] = (int*)malloc(minterms*tables * sizeof(int));
                for(int j = 0; j < counter2[i]; j++){
                    if(minimize[i][j] != -2){
                        index1[i][counter[i]] = minimize[i][j];
                        index2[i][counter[i]++] = minimize[i][j];
                    }
                }
                for(int j = 0; j < counter_mini2[i]; j++){
                    if(minimize2[i][j] != -2){
                        index3[i][counter3[i]++] = minimize2[i][j];
                    }
                }
            }
            minimize = (int**) realloc(minimize, (var+1-tables)*sizeof(int*));
            minimize2 = (int**) realloc(minimize2, (var+1-tables)*sizeof(int*));
            counter2 = (int*)realloc(counter2, (var+1-tables)*sizeof(int));
            counter_mini2 = (int*)realloc(counter_mini2, (var+1-tables)*sizeof(int));
            for(int i = 0; i < var+1-tables; i++){
                minimize[i] = (int*)malloc(minterms*(tables+1)*sizeof(int));
                minimize2[i] = (int*)malloc(minterms*(tables+1)*sizeof(int));
                counter2[i] = 0;
                counter_mini2[i] = 0;
            }
}
void initialize(){
    int temp, ones;
    printf("Enter minterms: ");
    for(int i = 0; i < minterms; i++){
        scanf("%d", &temp);
        //terms[i] = temp;
        ones = numberOfOnes(temp);
        index1[ones][counter[ones]] = temp;
        index2[ones][counter[ones]++] = temp;
        index3[ones][counter3[ones]++] = temp;

    }
}
void print_table(int tables){
    int k = 0;
    printf("table: %d\n", tables);
        for(int i = 0; i < var+2-tables; i++){
            for(int j = 0; j < counter3[i]; j++){
                printf("%d ", index3[i][j]);
                k++;
                if(k == pow(2, tables-1)){
                    printf("\n");
                    k = 0;
                }

            }
            printf("_________\n");
        }
}
void create_PI(int tables){
    int row = 0;
    for(int i = 0; i < var+2-tables; i++){
        row = 0;
        for(int j = 0; j < counter[i]; j += tables){
            if(index2[i][j] != -1) {
                for(int k = 0; k < tables; k++){
                    prime_implicants[size_pi++] = index2[i][j+k];
                    prime_implicants = (int*)realloc(prime_implicants, (size_pi+10)*sizeof(int));
                }
                for(int k = row*pow(2, tables-1); k < pow(2, tables-1)*(row+1); k++){
                    terms_pi[size_terms_pi++] = index3[i][k];
                    //terms_pi = (int*)realloc(terms_pi, (size_terms_pi+10)*sizeof(int));
                }
                counter_pi[size_counter_pi] = tables;
                cost[size_counter_pi] = var-tables+2;
                counter_pi = (int*)realloc(counter_pi, (size_counter_pi+10)*sizeof(int));
                counter_terms_pi[size_counter_pi++] = pow(2, tables-1);
                counter_terms_pi = (int*)realloc(counter_terms_pi, (size_counter_pi+10)*sizeof(int));

            }
            row++;
        }
    }

}
int logBase2(int n){
    int i = 0;
    while((1 << i) != n){
        i++;
    }
    return i;
}
void convertToCharacter(int arr[], int counter){
    int c = 0; char letter = 'A';
    numberOfOnes(arr[0]);
    for(int j = var-1; j >= 0; j--){
        c = 0;
        for(int k = 1; k < counter; k++){
            if(j == logBase2(arr[k])){
                c = 1;
                break;
            }
        }
        if(!c){
            printf("%c", letter);
            if(!binary[j]) printf("'");
        }
        letter++;
    }
}
void print_pi(){
    printf("Number of Prime Implicants: %d\n", size_counter_pi);
    printf("Prime Implicants:\n");
    int a = 0;
    for(int i = 0; i < size_pi; i+= counter_pi[a-1]){
        int temp[counter_pi[a]];
        for(int j = 0; j < counter_pi[a]; j++){
            temp[j] = prime_implicants[j+i];
        }
        convertToCharacter(temp, counter_pi[a]);
        printf("\n");
        a++;
    }

}
void minimization(){
    int matching = 1, tables = 1, pairs = 0, rowj, rowk;
    while(1){
        print_table(tables);
        matching = 0;
        for(int i = 0; i < var+1-tables; i++){
            rowj = -1;
            for(int j = 0; j < counter[i]; j += tables){
                rowj++; rowk = -1;
                for(int k = 0; k < counter[i+1]; k += tables){
                    pairs = 0; rowk++;
                    if(numberOfOnes(index1[i+1][k] - index1[i][j]) == 1){
                        for(int l = 1; l < tables; l++){
                            if(index1[i][j+l] == index1[i+1][k+l]){
                                pairs++;
                            }
                            else break;
                        }
                        if(pairs == tables-1){
                            for(int l = 0; l < tables; l++){
                                minimize[i][counter2[i]++] = index1[i][j+l];
                                if(l == 0) continue;
                                index2[i][j+l] = -1;
                                index2[i+1][k+l] = -1;
                            }
                            minimize[i][counter2[i]++] = index1[i+1][k] - index1[i][j];
                            for(int l = rowj*pow(2, tables-1); l < (rowj+1)*pow(2, tables-1); l++){
                                minimize2[i][counter_mini2[i]++] = index3[i][l];
                            }
                            for(int l = rowk*pow(2, tables-1); l < (rowk+1)*pow(2, tables-1); l++){
                                minimize2[i][counter_mini2[i]++] = index3[i+1][l];
                            }
                            index2[i+1][k] = -1;
                            index2[i][j] = -1;
                            matching = 1;
                        }
                    }
                }
            }
        }
        filteration(tables+1);
        create_PI(tables);

        if(matching){
            tables++;
            alternate(tables);

        }

        else{
            printf("exit\n");
            break;
        }
    }
    print_pi();
}
void print_EPI(){
    printf("Number of Essential Prime Implicants: %d\n", size_counter_epi);
    printf("Essential Prime implicants:\n");
    int a = 0;
    for(int i = 0; i < size_epi; i+= counter_epi[a-1]){
        int temp[counter_epi[a]];
        for(int j = 0; j < counter_epi[a]; j++){
            temp[j] = EPI[j+i];
        }
        convertToCharacter(temp, counter_epi[a]);
        printf("\n");
        a++;
    }
}
void create_EPI(){
    int sum = 0, k = 0;
//    for(int i = 0; i < size_terms_pi; i++){
//        printf("%d ", terms_pi[i]);
//        terms[terms_pi[i]]++;
//    }
//    printf("\n");
//    for(int i = 0; i < size_counter_pi; i++) printf("%d ", cost[i]);
//    printf("\n");
//    for(int i = 0; i < pow(2, var); i++) printf("%d ", terms[i]);
//    printf("\n");
    for(int i = 0; i < pow(2, var); i++){
        if(terms[i] == 1){
            for(int j = 0; j < size_terms_pi; j++){
                if(terms_pi[j] == i){
                    sum = 0; k =0;
                    while(sum <= j) sum += counter_terms_pi[k++];
                    k--;
                    for(int l = sum - counter_terms_pi[k]; l < sum; l++) {
                        terms[terms_pi[l]] = 0;
                        terms_pi[l] = -2;
                    }
                    int x = 0; sum = 0;
                    while(x < k) sum += counter_pi[x++];
                    for(int l = sum; l < sum+counter_pi[x]; l++){
                        EPI[size_epi++] = prime_implicants[l];
                        EPI = (int*) realloc(EPI, (size_epi+1)*sizeof(int));
                    }
                    counter_epi[size_counter_epi++] = counter_pi[x];
                    counter_epi = (int*)realloc(counter_epi, (size_counter_epi+1)*sizeof(int));
                    break;
                }
            }
        }
    }

}

void print_function(){
    printf("Result: f = ");
    int a = 0;
    for(int i = 0; i < size_epi; i+= counter_epi[a-1]){

        int temp[counter_epi[a]];
        for(int j = 0; j < counter_epi[a]; j++){
            temp[j] = EPI[j+i];

        }

        convertToCharacter(temp, counter_epi[a]);
        if(i+counter_epi[a] < size_epi) printf(" + ");

        a++;

    }
}
void row_dominance(){
    int a = 0; int b = 1; int d1 = 0; int d2 = 0;
    for(int i = 0; i < size_terms_pi - counter_terms_pi[size_counter_pi-1]; i += counter_terms_pi[a-1]){
        if(terms_pi[i] == -2) {
            a++;
            b = a+1;
            continue;
        }
        d1 = 0;
        terms_1 = (int*)calloc(pow(2, var), sizeof(int));
        for(int j = 0; j < counter_terms_pi[a]; j++){
            if(terms[terms_pi[i+j]] != 0) terms_1[terms_pi[i+j]]++;
        }
        for(int x = i + counter_terms_pi[b]; x < size_terms_pi; x += counter_terms_pi[b-1]){
            if(terms_pi[x] == -2) continue;
            d2 = 0;
            terms_2 = (int*)calloc(pow(2, var), sizeof(int));

            for(int y = 0; y < counter_terms_pi[b]; y++){
                if(terms[terms_pi[x+y]] != 0) terms_2[terms_pi[x+y]]++;
            }
            for(int l = 0; l < pow(2, var); l++){
                if(terms_1[l] >= terms_2[l]) d1++;
                else if(terms_2[l] >= terms_1[l]) d2++;
            }
            if(d1 == pow(2, var) && cost[a] <= cost[b]){
                printf("row %d dominates row %d\n", a, b);
                for(int y = 0; y < counter_terms_pi[b]; y++){
                    if(terms[terms_pi[x+y]] != 0) terms[terms_pi[x+y]]--;
                    terms_pi[x+y] = -2;
                }
            }
            else if(d2 == pow(2, var) && cost[b] <= cost[a]){
                printf("row %d dominates row %d\n", b, a);
                for(int j = 0; j < counter_terms_pi[a]; j++){
                    if(terms[terms_pi[i+j]] != 0) terms[terms_pi[j+i]]--;
                    terms_pi[i+j] = -2;
                }
            }

            b++;
        }
        a++;
        b = a+1;
    }
}
int booleanMultiplierK(int n)
{
    int i,j,k;
    static char tmp[5000] = "",selected[100];
    static int strCheckPoint = 0;

    for(k = 0 ; k < strlen(str[0]) ; k += n)
    {
        for(i = k ; i < n + k ; i++)
        {
            selected[i - k] = str[0][i];
        }
        //printf("%s\n",selected);

        for(j = 0 ; j < strlen(str[1]) ; j++)
        {
            strcat(tmp,selected);
            tmp[n + j + (j * strlen(selected)) + strCheckPoint] = str[1][j];
            tmp[n + j + (j * strlen(selected)) + strCheckPoint+1] = '\0';

        }
        strCheckPoint = strlen(tmp);
    }

    strcpy(str[0],tmp);

    n += 1;
    if(n == (1 << var)-covered_minterms)
    {
        //printf("%s\n", str[0]);
        return n;
    }
    strCheckPoint = 0;
    strcpy(str[1],str[n]);

    booleanMultiplierK(n);

}
void petrick(){
    int a = 0; char letter[2] = "a"; int w = 0; int n; int c = 0; int min = 10000;
    for(int i = 0; i < pow(2, var); i++){
        if(terms[i] != 0){
            a = 0;

            for(int x = 0; x < size_terms_pi; x += counter_pi[a-1]){
                letter[0] = 'a' + a;
                for(int y = 0; y < counter_pi[a]; y++){
                    if(terms_pi[x+y] == i){
                        strcat(str[w], letter);
                        counter_str[w]++;
                        break;
                    }
                }
               a++;

            }
            w++;
        }
    }
    //strcpy(str[pow(2, var)-covered_minterms], "");
    n = booleanMultiplierK(1);

    int isRepeated = 0; int r = 0;
    for(int i = 0; i < strlen(str[0]); i += n){
        c = 0;
        for(int j = 0; j < n; j++){
            isRepeated = 0;
            if(j == 0){
                c += cost[str[0][i+j] - 'a'];
                continue;
            }
            for(int k = j - 1; k >= 0; k--){
                if(str[0][i+k] == str[0][i+j]){
                    isRepeated = 1;
                    break;
                }
            }
            if(!isRepeated){
                c += cost['a' - str[0][i+j] + size_counter_pi];
            }
        }
        if(c < min) {
            min = c;
            r = i;
        }
    }
    petrick_arr = (char*)malloc(n*sizeof(char));
    petrick_arr[counter_petrick_arr++] = str[0][r];
    for(int i = 1; i < n; i++){
        isRepeated = 0;
        for(int j = i-1; j >= 0; j--){
            if(str[0][r+i] == str[0][r+j]) {
                isRepeated = 1;
                break;
            }
        }
        if(!isRepeated){
            petrick_arr[counter_petrick_arr++] = str[0][r+i];
        }
    }
    int temp;
    int sum = 0; int k;
    for(int i = 0; i < counter_petrick_arr; i++){
        sum = 0;
        k = 0;
        while(k <= petrick_arr[i] - 'a') {
            sum += counter_terms_pi[k++];
        }
        k--;
        //printf("%d ", sum);
        for(int j = sum-counter_terms_pi[k]; j < sum; j++){
            //printf("%d ", terms_pi[j]);
            terms[terms_pi[j]] = 1;
        }
        int x = 0; sum = 0;
        while(x < k) sum += counter_pi[x++];
        for(int l = sum; l < sum+counter_pi[x]; l++){
            //printf("%d ", prime_implicants[l]);
            EPI[size_epi++] = prime_implicants[l];
            EPI = (int*) realloc(EPI, (size_epi+1)*sizeof(int));
        }
        counter_epi[size_counter_epi++] = counter_pi[k];
        counter_epi = (int*)realloc(counter_epi, (size_counter_epi+1)*sizeof(int));
    }
    //create_EPI();
    printf("\n");
}
int main()
{
    printf("Enter number of variables: ");
    scanf("%d", &var);
    printf("Enter number of minterms: ");
    scanf("%d", &minterms);

    terms = (int*) calloc(pow(2, var), sizeof(int));
    index1 = (int**) malloc((var+1) * sizeof(int*));
    index2 = (int**) malloc((var+1) * sizeof(int *));
    index3 = (int**) malloc((var+1) * sizeof(int*));
    for(int i = 0; i < var+1; i++){
        index1[i] = (int*)malloc(minterms * sizeof(int));
        index2[i] = (int*) malloc(minterms * sizeof(int));
        index3[i] = (int*) malloc(minterms * sizeof(int));
    }

    minimize = (int**)malloc(var*sizeof(int*));
    minimize2 = (int**)malloc(var*sizeof(int*));
    for(int i = 0; i < var; i++){
        minimize[i] = (int*)malloc(minterms*2*sizeof(int));
        minimize2[i] = (int*)malloc(minterms*2*sizeof(int));
    }

     counter = (int*) calloc(var+1, sizeof(int));
     counter2 = (int*)calloc(var, sizeof(int));
     counter3 = (int*) calloc(var+1, sizeof(int));
     counter_mini2 = (int*) calloc(var, sizeof(int));

     prime_implicants = (int*) malloc(var*sizeof(int));
     cost = (int*)malloc(size_counter_pi * sizeof(int));
     terms_pi = (int*) malloc(100*sizeof(int));
     EPI = (int*) malloc(var*sizeof(int));
     counter_pi = (int*) malloc(minterms * sizeof(int));
     counter_terms_pi = (int*) malloc(minterms * sizeof(int));
     counter_epi = (int*) malloc(minterms * sizeof(int));

     initialize();
     minimization();
    for(int i = 0; i < size_terms_pi; i++){
        terms[terms_pi[i]]++;
    }
    create_EPI();
    print_EPI();

     //Row and Column dominance
    terms_1 = (int*) calloc(pow(2, var), sizeof(int));
    terms_2 = (int*) calloc(pow(2, var), sizeof(int));

    for(int i = 0; i < pow(2, var); i++){
        //printf("%d ", terms[i]);
        if(terms[i] == 0) {
            covered_minterms++;
        }
    }
    printf("\n");
    if(covered_minterms == pow(2, var)){
        print_function();
        return 0;
    }
    row_dominance();
    create_EPI();
    covered_minterms = 0;
    for(int i = 0; i < pow(2, var); i++){
        //printf("%d ", terms[i]);
        if(terms[i] == 0) {
            covered_minterms++;
        }
    }
    printf("\n");
    if(covered_minterms == pow(2, var)) {
        print_function();
        return 0;
    }
        str = (char**)malloc((pow(2, var) - covered_minterms+1)*sizeof(char*));
        counter_str = (int*)calloc(pow(2, var) - covered_minterms, sizeof(int));
        for(int i = 0; i < pow(2, var) - covered_minterms; i++){
            str[i] = (char*)malloc(5000*sizeof(char));
            for(int j = 0; j < 5000; j++) str[i][j] = (char) 0;
        }
        //strcpy(str[(int)pow(2, var) - covered_minterms], "");
        printf("##########PETRICK METHOD####################\n");
        petrick();
        print_function();
    return 0;
}
