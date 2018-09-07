#include <TFT_ST7735.h>

#define GREEN  0x07E0
#define BLACK 0x0000
#define RED  0x001F

const uint8_t steps = 10, population = 5;
uint8_t x, y,Fx = 122, Fy = 5, step_size = 16, generation = 0, Max_in, Max2_in, Min_in;
boolean first = 1, solution = 0;

uint8_t world_genes[population][steps], fit[population];

uint8_t fitness(uint8_t x, uint8_t y) {
  uint8_t d = round(sqrt(sq(64-x)+sq(0-y)));
  if(d>=255){
    d = 255;
   }else if(d<=0){d = 0;}
  return 255 - d;
}

TFT_ST7735 tft = TFT_ST7735();

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));
  tft.init();
  tft.invertDisplay(true);
  tft.fillScreen(BLACK);
  tft.fillCircle(Fx, Fy, 3, RED);
  start();
}

void start(){
  for (uint8_t i = 0; i< population; i++){
    x = 60, y = 158;
    for(uint8_t j = 0; j< steps; j++){
      if(first){
        world_genes[i][j] = random(30,150);
      }
      xyUpdate(i, j);
    }
    fit[i] = fitness(x, y);
    if(fit[i] == 0){Serial.println("best solution found"); exit(0);}
  }
  first = 0;
  Serial.println("generation" + generation++);
  delay(100);
  selection();
  crossover(Max_in,Max2_in);
  mutation();
  //newPop();
  tft.fillScreen(BLACK);
  tft.fillCircle(Fx, Fy, 3, RED);
  start();
}

void selection(){
  uint8_t fitMax, fitMax2;
  if(fit[0] > fit[1]) {
   fitMax2 = fit[1];
   fitMax = fit[0];
   Max_in = 0, Max2_in = 1;
  } else {
   fitMax2 = fit[0];
   fitMax = fit[1];
   Max_in = 1, Max2_in = 0;
  }
  for(uint8_t i = 2; i < population; i++){
    if(fit[i] >= fitMax){  
        fitMax2=fitMax;
        fitMax=fit[i];
        Max_in = i;          
    }
    else if(fit[i] > fitMax2){
        fitMax2=fit[i];
        Max2_in = i;
    }
  }
  uint8_t fitMin = fit[0];
  for(uint8_t i=1;  i < population;  i++ ){
    if ( fit[i] < fitMin ){
       fitMin = fit[i];
       Min_in = i;
    }
  }
}

void crossover(uint8_t parent1,uint8_t parent2){
  uint8_t cr_rn = random(0,steps);
    for(uint8_t i = 0; i < cr_rn; i++){
      uint8_t temp = world_genes[parent1][i];
      world_genes[parent1][i]= world_genes[parent2][i];
      world_genes[parent2][i] = temp;
    }  
}

void newPop(){
  for(uint8_t j = 0; j< steps; j++){
    world_genes[Min_in][j] = random(30,150);
  }
}

void mutation(){
  
}

//void evolution(){
//  uint8_t fitAvg, fitMax = 0, j = 0, i = 0,k = 0, cross_gen[population][steps];
//  for(i = 0; i < population; i++){
//    fitAvg += fit[i];
//    if(fit[i] > fitMax) {fitMax = fit[i];}
//  }
//  fitAvg/= population;
//  Serial.println(fitAvg);
//  if(fitAvg == fitMax){fitAvg>>=1;}
//  for(i = 0; i < population; i++){
//    if(fit[i] >= fitAvg){for(k = 0; k < population; k++){cross_gen[j++][k] = world_genes[i][k]; Serial.println(cross_gen[j][k]);}}  
//  }
//  delay(100);
//  for(i = 0; i < j;i+2){
//    uint8_t cr_rn = random(2,population-2);
//    for(k = 0; k < cr_rn; cr_rn++){
//      uint8_t temp = cross_gen[i][k];
//      cross_gen[i][k]= cross_gen[i+1][k];
//      cross_gen[i+1][k] = temp;
//    }  
//  }
//  delay(100);
//  if(j == population){j>>=1;}
//  while(j < population){
//    for(i = 0; i< steps; i++){
//      cross_gen[j++][i] = random(30,150)*(PI/180);
//    }
//  }
//  for (uint8_t i = 0; i< population; i++){
//    x = 125, y = 158;
//    for(uint8_t j = 0; j< steps; j++){
//      world_genes[i][j] = cross_gen[i][j];
//      xyUpdate(i, j);
//    }
//    fit[i] = fitness(x, y);
//    if(!fit[i]){Serial.println("best solution found");}
//    else{generation++; evolution();}
//  }
//}

void xyUpdate(uint8_t i, uint8_t j){
  uint8_t prev_x = x, prev_y = y;
  x = prev_x-(step_size*cos(world_genes[i][j]*(PI/180)));
  y = prev_y-(step_size*sin(world_genes[i][j]*(PI/180)));
  tft.drawLine(prev_x, prev_y, x, y, GREEN);
}

void loop() {
  delay(10);
}
