//#include <iostream>
#include <bits/stdc++.h>
#include <windows.h>
//#include <cstdio>
using namespace std;

void identity(){
    printf("Rahmad Bisma Zulfi Pahlevi\n5025231290\n\n");
}

class Robot{
    private:
        int spos_x, spos_y;// Start posisi robot
        int msize_x, msize_y;// Ukuran peta
        int fpos_x=-1, fpos_y=-1;// Finish
        char og_map[100][100];

        //Varible tambahan untuk algoritma pencarian
        int branches_count=0;
        int branches_at_order[1000];
        int branches_x[1000];
        int branches_y[1000];

    public:
        int pos_x=-1, pos_y=-1; //Posisi robot
        char map[100][100];
        int len_order=0;
        char order[1000];
        
        int finish_state=0;

        //Mengassign posisi robot sesuai peta
        void set_pos(int x, int y){
            pos_x=x;
            spos_x=x;
            pos_y=y;
            spos_y=y;
        }

        //Mengassign peta ke robot
        void setmap(int x_size, int y_size, char temp[100][100]){
            //reset finish pos
            fpos_x=-1;
            fpos_y=-1;

            msize_x=x_size; 
            msize_y=y_size;
            for(int i=0;i<x_size;i++){
                for(int j=0;j<y_size;j++){
                    map[i][j]=temp[i][j];
                    og_map[i][j]=temp[i][j];
                    if(map[i][j]=='*') set_pos(i, j);
                }
            }
            if(pos_x==-1 || pos_y==-1) printf("[-] Lokasi robot nggak jelas");
        }

        //Mereset kembali peta ke aslinya untuk search ulang
        void reset_map(){//------------------------------------------------- for search func
            for(int i=0;i<msize_x;i++){
                for(int j=0;j<msize_y;j++){
                    map[i][j]=og_map[i][j];
                    if(map[i][j]=='*') set_pos(i, j);
                }
            }
         }

        //Mencetak seluruh isi peta
        void cetak_in(){
            for(int i=0;i<msize_x;i++){
                for(int j=0;j<msize_y;j++){
                    printf("%c", map[i][j]);
                }
                printf("\n");
            }
        }

        int check_valid_move(int cpos_x, int cpos_y, int count, int mode){
            //printf("\nx1\n");
            if(map[cpos_x][cpos_y]=='$'){

                //printf("oke okeokeokeokeok\n");
                //printf("Robot Mencapai Finish\n"); //finish
                finish_state=1;
                fpos_x=cpos_x;
                fpos_y=cpos_y;
                return 1;
            }
            if(cpos_x > msize_x-1 || cpos_x < 0 || cpos_y > msize_y-1 || cpos_y < 0){
                if(mode) printf("[-] %d Robot Menabrak Tembok\n", count);
                return 0;
            }
            else if(map[cpos_x][cpos_y]=='o' || map[cpos_x][cpos_y]=='#'){
                if(mode) printf("[-] %d Robot Menabrak Obstacle\n", count);
                return 0;
            }
            else return 1;
        }

        void move(char x, int count, int mode, int print_it){
            int temp_x=pos_x, temp_y=pos_y; //take current postion
            if(x=='w'){
                if(check_valid_move(pos_x-1, pos_y, count, mode)){ //-- mode untuk mencetak apakah robot menabrak sesuatu
                    pos_x-=1;
                    map[temp_x][temp_y]='#';
                }
            }
            else if(x=='a'){
                if(check_valid_move(pos_x, pos_y-1, count, mode)){
                    pos_y-=1;
                    map[temp_x][temp_y]='#';
                }
            }
            else if(x=='s'){
                if(check_valid_move(pos_x+1, pos_y, count, mode)){
                    pos_x+=1;
                    map[temp_x][temp_y]='#';
                }
            }
            else if(x=='d'){
                if(check_valid_move(pos_x, pos_y+1, count, mode)){
                    pos_y+=1;
                    map[temp_x][temp_y]='#';
                }
            }
            map[pos_x][pos_y]='*';
            if(print_it) cetak_in();
        }

        void take_order(){
            system("cls");
            identity();

            printf("[?] Masukan Banyak Perintah: ");
            scanf("%d", &len_order);
            getchar();
            printf("[?] Masukan Perintah: ");
            for(int i=0;i<len_order;i++) scanf("%c", &order[i]);
            system("cls");
            for(int i=0;i<len_order;i++){
                sleep(1000);
                if(!finish_state) move(order[i], i, 1, 0); // order[i] - perintah, i - perintah ke i, 1 - memberi status apakah robot menabrak, 0 - untuk tidak mencetak hasil gerakan
                else break;
            }
            cetak_in();
            if(finish_state) printf("[+] Robot Mencapai Finish\n");
            if(finish_state==0){
                printf("[-] Robot Tidak Mencapai Finish\n");
                Sleep(2000);
                getchar();
                printf("[?] Apakah kamu ingin robot mencari sendiri.......(y/n): ");
                int choice=getchar();
                if(choice=='y') search_path();
                else exit(0);
            }
            finish_state=0;
        }

        void exec_order(char order[]){ //---------------untuk search
            system("cls");
            for(int i=0;i<len_order;i++){
                if(!finish_state) move(order[i], i, 1, 0); // order[i] - perintah, i - perintah ke i, 1 - memberi status apakah robot menabrak, 0 - untuk tidak mencetak hasil gerakan
                else break;
            }
            cetak_in();
            if(finish_state) printf("[+] Robot Mencapai Finish\n");
            if(finish_state==0){
                printf("[-] Robot Tidak Mencapai Finish\n");
                Sleep(2000);
                getchar();
                printf("[?] Apakah kamu ingin robot mencari sendiri.......(y/n): ");
                int choice=getchar();
                if(choice=='y') search_path();
                else exit(0);
            }
            finish_state=0;
        }

        void reset_order(char order[], int *len_order){
            for(int i=0;i<*len_order;i++){
                order[i]='0';
                // for(int j=0;j<len_order;j++){ menghapus dengan overwrite data dengan data selanjutnya
                //     order[j]=order[j+1];
                // }
            }
            *len_order=0;
        }
        //----------------------------------------
        //pencarian otomatis 
        int check_can_move(int x, int y){ //on v2 is unused
            //printf("\nx1\n");
            if(map[x-1][y]=='$' || map[x][y-1]=='$' || map[x+1][y]=='$' || map[x][y+1]=='$'){
                finish_state=0;
                return 1;
            }
            if(check_valid_move(x-1, y, 0, 0) || check_valid_move(x, y-1, 0, 0) || check_valid_move(x+1, y, 0, 0) || check_valid_move(x, y+1, 0, 0) || map[x][y]=='$') return 1;
            else {
                //map[x][y]='#';
                return 0;
            }
        }

        int check_finish(int x, int y){
            //printf("1");
            if(fpos_x!=-1 && fpos_y!=-1){
                if(x==fpos_x && y==fpos_y) {
                    finish_state==1;
                    return 1;
                }
                else {
                    finish_state=0;
                    return 0;
                }
            }
            else return 0;
        }

        void remove_part_order(int branches_order){
            for(int i=len_order-1;i>=5;i--) order[i]=order[i+1];
            len_order=branches_order;
        }

        void check_branches(int x, int y, int order_i){
            int branches=0;
            if(check_valid_move(x-1, y, 0, 0)) branches++;
            if(check_valid_move(x, y-1, 0, 0)) branches++;
            if(check_valid_move(x+1, y, 0, 0)) branches++;
            if(check_valid_move(x, y+1, 0, 0)) branches++;

            if(branches>1){
                branches_at_order[branches_count]=order_i;
                branches_x[branches_count]=x;
                branches_y[branches_count]=y;
                branches_count++; //bukan banyak cabang tapi cabang ke i
            }
        }

        void remove_used_branches(){
            branches_count--;
            branches_at_order[branches_count]=branches_at_order[branches_count+1];
            branches_x[branches_count]=branches_x[branches_count+1];
            branches_y[branches_count]=branches_y[branches_count+1];
        }

        int return_to_branches(){
            //printf("b%d %db\n", branches_x[branches_count-1], branches_y[branches_count-1]);
            //if(finish_state) return 0;
            map[pos_x][pos_y]='#';
            pos_x=branches_x[branches_count-1];
            pos_y=branches_y[branches_count-1];
            map[pos_x][pos_y]='*';
            remove_part_order(branches_at_order[branches_count-1]);
            remove_used_branches();
            //printf("b%d %db\n", branches_x[branches_count-1], branches_y[branches_count-1]);
            return 1;
        }

        int smove(char x){//fungsi move dibuat ulang
            if(finish_state==1 && pos_x == fpos_x && pos_y == fpos_y) return 0;
            //finish_state=0;
            if(check_finish(pos_x, pos_y)) return 0;
            //printf("3");
            //posisi awal
            int temp_x=pos_x;
            int temp_y=pos_y;

            check_branches(pos_x, pos_y, len_order);
            //int no_way_found=0;
            //printf("a%d %d %ca\n", pos_x, pos_y, x);

            if(x=='w'){
                if(check_valid_move(pos_x-1,pos_y, len_order, 0)){ //-- mode untuk mencetak apakah robot menabrak sesuatu
                    map[temp_x][temp_y]='#';
                    //if(!check_can_move(pos_x-1, pos_y)) return 0; //v1 //check further move
                    //len_order++;
                    pos_x-=1;
                    map[pos_x][pos_y]='*';
                    order[len_order++]=x;
                    return 1;
                }
                // else if(finish_state==0){
                //     return 0;
                    // no_way_found++;
                    // if(branches_count>0) no_way_found++;//return return_to_branches(); //v2
                    // else return 0;
                //}
                else return 0;
            }
            else if(x=='a'){
                if(check_valid_move(pos_x, pos_y-1, len_order, 0)){
                    map[temp_x][temp_y]='#';
                    //if(!check_can_move(pos_x, pos_y-1)) return 0;
                    //len_order++;
                    pos_y-=1;
                    map[pos_x][pos_y]='*';
                    order[len_order++]=x;
                    return 1;
                }
                // else if(finish_state==0){
                //     return 0;
                //     //no_way_found++;
                //     //if(branches_count>0) no_way_found++;//return return_to_branches(); //v2
                //     //else return 0;
                // }
                else return 0;
            }
            else if(x=='s'){
                if(check_valid_move(pos_x+1, pos_y, len_order, 0)){
                    map[temp_x][temp_y]='#';
                    //if(!check_can_move(pos_x+1, pos_y)) return 0;
                    //len_order++;
                    pos_x+=1;
                    map[pos_x][pos_y]='*';
                    order[len_order++]=x;
                    return 1;
                }
                // else if(finish_state==0){
                //     return 0;
                //     //no_way_found++;
                //     //if(branches_count>0) no_way_found++;//return return_to_branches(); //v2
                //     //else return 0;
                // }
                else return 0;

            }
            else if(x=='d'){
                if(check_valid_move(pos_x, pos_y+1, len_order, 0)){
                    map[temp_x][temp_y]='#';
                    //if(!check_can_move(pos_x, pos_y+1)) return 0;
                    //len_order++;
                    pos_y+=1;
                    map[pos_x][pos_y]='*';
                    order[len_order++]=x;
                    return 1;
                }
                else if(finish_state==0){
                    //return 0;
                    //no_way_found++;
                    if(branches_count>0) return return_to_branches(); //v2
                    else return 0;
                }
                else return 0;
            }
            else return 0;
            // if(branches_count>0) {
            //     if(no_way_found) return_to_branches();
            // }
            // else return 0;
        }
        
        int begin_search(){
            system("cls");
            identity();

            printf("[!] Robot tidak mengetahui dimana titik finish berada. Hanya bisa meninjau selangkah selanjutnya\n[!] Robot mulai mencari\n");
            //printf("2");
            //printf("%d\n", len_order);
            
            if(len_order>=99999) return 0;
            cetak_in();
            Sleep(500);
            if(smove('w') || smove('a') || smove('s') || smove('d')){
                //getchar();
                //printf("dddddddddddd\n");
                //cetak_in();
                //printf("aaaaaaaaaaa%d %d %d", finish_state, fpos_x, fpos_y);

                if(begin_search() && finish_state) return 1;
                
                else return 0;
            }
            else return 0;
        }

        // void begin_search2xz(){
        //     for(;;){
        //         if(smove('w')) continue;
        //         else if(smove('a')) continue;
        //         else if(smove('s')) continue;
        //         else if(smove('d')) continue;
        //     }
        // }
        
        void search_path(){// fungsi pencarian mengambil duplikat variable private pada class spos_x dan spos_y berupa posisi awal----------------------------------------------------------
            system("cls");
            reset_map(); // mereset peta ke awalnya
            reset_order(order, &len_order);
            //reset_order(order_avoid, &len_order);
            //int state_def=0; //pengganti return
            //int s_len_order=0;//counter perintah sementara
            //char s_order[1000]; //list perintah sementara
            printf("[!] Robot tidak mengetahui dimana titik finish berada. Hanya bisa meninjau selangkah selanjutnya\n[!] Robot mulai mencari\n");  

            if (begin_search() || finish_state) {
                system("cls");
                cetak_in();
                printf("[+] Robot menemukan titik finish pada titik %d %d\n",fpos_x, fpos_y);
                printf("[+] Perintah yang dihasilkan robot: ");
                for(int i=0;i<len_order;i++) printf("%c", order[i]);
            }
            else printf("[-] Finish Tidak Dapat Ditemukan Robot nya agak.....\n");
            //if(finish_state) printf("Ditemukan titik finish pada titik %d %d\n",fpos_x, fpos_y);           
            //atek reset map
        }
};

class Map{
    public:
        int size_x, size_y;
        char map[100][100];
        void create_map(){
            system("cls");
            identity();
            printf("Masukan Ukuran Peta: ");
            scanf("%d %d", &size_x, &size_y);
            getchar();
            printf("Masukan Peta:\n");
            for(int i=0;i<size_x;i++){
                for(int j=0;j<size_y;j++){
                    scanf("%c", &map[i][j]);
                }
                getchar();
            }
        }
};

//algoritma pencarian
//1 nggeleki finish
//2 podo ae tapi nyimpen cabang cabang e nek mentok mundur ndek cabang mburine ------- selesai
//optimisasi perintah e
//2.1 podo ae tapi njupuk info teko list order e cek apa onok cabang
//ngganti fungsi check_can_move yang meninjau ketersediaan langkah di langkah+1. diganti fungsi reset cabang
//dadi robot e dijarno mlaku mrunu nek mentok di reset dok cabang e kyok check point lah
//pola pencarian gak jelas karena mengikuti urutan prioritas cek arah yang mana W -> A -> S -> D
//note fungsi return to branches hanya dipanggil jika perintah w atau a atau s telah dicek untuk menghindari pemanggilan diluar seluruh kemungkinan 
//3.0 Optimisasi pencarian dengan info titik finish ---------- sek repot

//ngekeki ukuran fixed 100 dok matrix e


int main(){
    system("cls");
    //peta
    Map peta;
    peta.create_map();

    //robot
    int choice;
    Robot robot1;
    robot1.setmap(peta.size_x, peta.size_y, peta.map); //menginisialisasi di peta mana robot ditaruh

    printf("[1] Masukan Perintah.\n[2] Biarkan Robot Mencari Sendiri.\n[?] ");
    choice=getchar();
    if(choice=='1') robot1.take_order(); //mengambil perintah untuk robot // minus robot tidak bisa kembali ke petak yang telah dilewati
    else if(choice=='2') robot1.search_path();
    //printf("%d %d", robot1.pos_x, robot1.pos_y);

    return 0;
}