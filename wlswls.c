#include <wiringPi.h>
#include <stdio.h>
#include <lcd.h>
#include <stdlib.h>
#include <time.h> 
#include <pthread.h>

#define JUKE_NUM 3

#define KEYPAD_PB1 2 //KEYPAD 포트 BT1 핀 정의
#define KEYPAD_PB2 3 
#define KEYPAD_PB3 4
#define KEYPAD_PB4 17
#define KEYPAD_PB5 27
#define KEYPAD_PB6 22
#define KEYPAD_PB7 10
#define KEYPAD_PB8 9
#define KEYPAD_PB9 11
#define KEYPAD_PB10 5
#define KEYPAD_PB11 6
#define KEYPAD_PB12 13

#define MAX_KEY_BT_NUM 12           //최대 키패드 개수
const int KeypadTable[12] = { KEYPAD_PB1, KEYPAD_PB2, KEYPAD_PB3, KEYPAD_PB4, KEYPAD_PB5, KEYPAD_PB6,
KEYPAD_PB7, KEYPAD_PB8, KEYPAD_PB9, KEYPAD_PB10, KEYPAD_PB11, KEYPAD_PB12 }; //KEYPAD 핀 테이블 선언

int KeypadRead(void)
{
    int i, nKeypadstate;
    nKeypadstate = 0;
    for (i = 0; i < MAX_KEY_BT_NUM; i++)
    {
        if (!digitalRead(KeypadTable[i]))
        {
            nKeypadstate |= (1 << i);
        }
    }
    return nKeypadstate;
}

const int Keylist[12] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

int KEY(int Inputkey) {
    switch (Inputkey) {
    case 1:
        return Keylist[0];
        break;
    case 2:
        return Keylist[1];
        break;
    case 4:
        return Keylist[2];
        break;
    case 8:
        return Keylist[3];
        break;
    case 16:
        return Keylist[4];
        break;
    case 32:
        return Keylist[5];
        break;
    case 64:
        return Keylist[6];
        break;
    case 128:
        return Keylist[7];
        break;
    case 256:
        return Keylist[8];
        break;
    case 512:
        return Keylist[9];
        break;
    case 1024:
        return Keylist[10];
        break;
    case 2048:
        return Keylist[11];
        break;
    }
    return 0;
}

void play_drum(int key_value) {
    switch (key_value) {
    case 5:
        system("omxplayer -o local Kick.wav &");
        break;
    case 6:
        system("omxplayer -o local C_Hat.wav &");
        break;
    case 7:
        system("omxplayer -o local L_Crash.wav &");
        break;
    case 8:
        system("omxplayer -o local M_Tom.mp3 &");
        break;
    case 9:
        system("omxplayer -o local F_Tom.mp3 &");
        break;
    case 10:
        system("omxplayer -o local piano01_do.mp3 &");
        break;
    case 11:
        system("omxplayer -o local piano03_re.mp3 &");
        break;
    case 12:
        system("omxplayer -o local piano05_mi.mp3 &");
        break;
    }
}

int main(int argc, char** argv)
{
    if (wiringPiSetupGpio() == -1)              //GPIO 사용하기
        return 1;

    int i;
    int nKeypadstate = 0;

    int pre_val = 0, val = 0;                // 랜덤 중복 방지 변수
    unsigned int u = 0;

    int key_value[100] = { 0, };             //key 값을 받는 배열
    int del[100] = { 0, };                  //key 입력 사이 딜레이 ms를 받는 배열

    int KEYPAD_PB[12];
    int KEYPAD_PB_old[12];

    srand(time(NULL));

    for (i = 0; i < MAX_KEY_BT_NUM; i++) {  //핀모드 설정
        pinMode(KeypadTable[i], INPUT);
    }

    for (i = 0; i < MAX_KEY_BT_NUM; i++)    //이전 버튼값을 현재 버튼값으로 초기화 (중복 입력 방지)
        KEYPAD_PB_old[i] = KEYPAD_PB[i];

    while (1) //전체 프로그램 시작
    {
        nKeypadstate = KeypadRead();
        printf("%d \n", nKeypadstate);//test
        for (i = 0; i < MAX_KEY_BT_NUM; i++) {
            KEYPAD_PB[i] = digitalRead(KeypadTable[i]);
        }

        printf("모드를 선택하세요!\n1. 런치패드\n2. 주크박스\n3. 녹음모드\n4. 녹음재생\n");
        delay(100);
        system("clear");

        // if KEYPAD_PB1 pressed
        if (!KEYPAD_PB[0] && KEYPAD_PB_old[0]) { //1번 버튼   런치패드 시작
            int pre = 0, cur = 0;
            printf("Launchpad Mode\n");
            while ((KEY(nKeypadstate) != 2) && (KEY(nKeypadstate) != 3) && (KEY(nKeypadstate) != 4)) //다른 2,3,4 번 메뉴 선택 시 종료
            {
                nKeypadstate = KeypadRead();
                cur = nKeypadstate;

                if ((pre != cur) && (cur != 0)) {
                    play_drum(KEY(nKeypadstate));
                    printf("%d \n", nKeypadstate);
                }

                pre = cur;
                delay(1);
            }
        }

        // if KEYPAD_PB2 pressed
        if (!KEYPAD_PB[1] && KEYPAD_PB_old[1]) {   //랜덤곡 재생 (한 번 더 누르면 종료)

            if (u % 2 == 0) {
                system("pkill omxplayer");  //재생 중인 노래가 있으면 정지

                printf("Random Playing Music\n");

                while (pre_val == val) { //중복 곡 재생 방지
                    val = rand() % 19;
                }

                if (val == 0) {
                    system("omxplayer -o local 바위섬.mp3 &");
                }
                else if (val == 1) {
                    system("omxplayer -o local 니가뭔데.mp3 &");
                }
                else if (val == 2) {
                    system("omxplayer -o local 디지몬.mp3 &");
                }
                else if (val == 3) {
                    system("omxplayer -o local Fire.mp3 &");
                }
                else if (val == 4) {
                    system("omxplayer -o local 야곱의축복.mp3 &");
                }
                else if (val == 5) {
                    system("omxplayer -o local 잘자요굿나잇.mp3 &");
                }
                else if (val == 6) {
                    system("omxplayer -o local Y.mp3 &");
                }
                else if (val == 7) {
                    system("omxplayer -o local 팝송.mp3 &");
                }
                else if (val == 8) {
                    system("omxplayer -o local NAYEONPOP.mp3 &");
                }
                else if (val == 9) {
                    system("omxplayer -o local PinkSweatHonesty.mp3 &");
                }
                else if (val == 10) {
                    system("omxplayer -o local Textmemerrychristmas.mp3 &");
                }
                else if (val == 11) {
                    system("omxplayer -o local RudolphtheRedNosedReindeer.mp3 &");
                }
                else if (val == 12) {
                    system("omxplayer -o local AllIWantForChristmasIsYou.mp3 &");
                }
                else if (val == 13) {
                    system("omxplayer -o local JingleBellRock.mp3 &");
                }
                else if (val == 14) {
                    system("omxplayer -o local SiaSnowman.mp3 &");
                }
                else if (val == 15) {
                    system("omxplayer -o local ArianaGrandeSantaTellMe.mp3 &");
                }
                else if (val == 16) {
                    system("omxplayer -o local LastChristmasWham.mp3 &");
                }
                else if (val == 17) {
                    system("omxplayer -o local 꽃길.mp3 &");
                }
                else if (val == 18) {
                    system("omxplayer -o local 사건의지평선.mp3 &");
                }
            }
            else {                          //정지
                system("pkill omxplayer");  //재생 중인 노래가 있으면 정지

                printf("Music Stop\n");
            }
            u++;                            //정지, 재생을 위한 증가 연산 
            pre_val = val;                  //이전 값에 현재 값 대입 ( 중복 입력 검사를 위함 )
        }//KEYPAD PB2 

        // if KEYPAD_PB3 pressed
        if (!KEYPAD_PB[2] && KEYPAD_PB_old[2]) { //3번 버튼 녹음 모드 ( 한 번 더 누르면 녹음 완료 )
            printf("Record Mode \n");
            system("pkill omxplayer");          //재생중인 노래가 있으면 정지

            system("clear");
            for (int i = 3; i > 0; i--) {
                printf("%d초 후 녹음이 시작됩니다.\n", i);
                delay(1000);
                system("clear");
            }
            printf("녹음시작!\n");                         //입력 오류 방지 delay

            unsigned int i = 0, j = 0;  //키 값 순서 i, 딜레이 ms 반복문 변수
            KEYPAD_PB_old[2] = KEYPAD_PB[2];

            int pre = 0, cur = 0;
            printf("start record\n");
            while ((KEY(nKeypadstate) != 1) && (KEY(nKeypadstate) != 2) && (KEY(nKeypadstate) != 4)) //다른 메뉴 선택 시 종료
            {
                nKeypadstate = KeypadRead();
                cur = nKeypadstate;

                if ((pre != cur) && (cur != 0)) {
                    play_drum(KEY(nKeypadstate));
                    printf("%d \n", nKeypadstate);
                    printf("Key(nKey) = %d\n", KEY(nKeypadstate));
                    key_value[i] = KEY(nKeypadstate);
                    del[i] = j; i++; j = 0;
                }
                pre = cur;

                if (KEY(nKeypadstate) == 3) {                            //한 번 더 3번 누르면 녹음 종료
                    printf("녹음종료\n"); del[i] = j; j = 0; i = 0;
                    delay(1000);
                    break;
                }
                delay(1); j++;
            }
        }//KEYPAD_PB3

        // if KEYPAD_PB4 pressed
        if (!KEYPAD_PB[3] && KEYPAD_PB_old[3]) { //4번 버튼 녹음 모드, (누르면 3번 녹음했던거 재생)
            printf("Play Recorded\n");
            system("pkill omxplayer");          //재생중인 노래가 있으면 정지

            for (int k = 0; k < 100; k++) {              //녹음 재생
            //omxplayer key_value[k]; delay(del[k]);
                if (key_value[k] == 0) { continue; }    // 입력값 없으면 continue
                printf("delay: %d \n", del[k]);
                delay(del[k]);
                //printf("play: %d \n", key_value[k]);          //추후 소리 재생으로 변경
                play_drum(key_value[k]);
            }//녹음 재생
        }// if KEYPAD_PB4 pressed

        for (i = 0; i < MAX_KEY_BT_NUM; i++)
            KEYPAD_PB_old[i] = KEYPAD_PB[i];
    }
    return 0;
}
