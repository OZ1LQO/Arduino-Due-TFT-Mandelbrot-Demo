// OZ1LQO Arduino DUE TFT Mandelbrot DEMO - Mandelbrot Ver. 3!
// EDR SummerCamp, 2014.07.13
// Based on demo code from Adafruit and my Mandelbrot rev. 2 code originally written in Python
//And knowledge from my initial mandelbrot experiments in the late 80'es

#include <Adafruit_GFX.h>    // Core graphics library
#include <tftlib.h> // Hardware-specific library
#include <ads7843.h> // Include touch
#define PSTR(a)  a
// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

//Touch Defines
#ifdef ELECFREAKS_TFT_SHIELD_V2
#define DCLK     6
#define CS       5  
#define DIN      4 
#define DOUT     3
#define IRQ      2 
#else
/** Elechouse TFT shield pin map */
#define DCLK     25
#define CS       26 
#define DIN      27 
#define DOUT     29
#define IRQ      30
#endif 




// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//Define LCD
TFTLCD tft;
//Define Touch
ADS7843 touch(CS, DCLK, DIN, DOUT, IRQ);
Point p;

void setup() {
  
  tft.begin();
  touch.begin();
  
  }

//Calculation engine for each cell
int mandel (double cx, double cy, int maxiter){
  
    int iteration=0;
    double x1=0;
    double y1=0;
    double x2=0;
    double y2=0;
    double temp=0;

    while (iteration<=maxiter && (x2+y2)<10000){
      
        temp=x2-y2+cx;
        y1=(2*x1*y1)+cy;
        x1=temp;
        x2=x1*x1;
        y2=y1*y1;
        iteration+=1;

    }
    return iteration;
}

//Calculation engine for the entire plot
//Takes coordinates as parameters, iterates through the display and plots the selected area
int mandelplot(double Xneg, double Xpos, double Yneg, double Ypos, int maxiter, int wx, int hy){
//reset iteration count and setup display
  int count=0;
  tft.fillScreen(BLACK);
  tft.setCursor(0,0);
  
for (int x=0; x<wx; x++){
    for (int y=0; y<hy; y++){
      //Calculate actual coordinate from X and Y boundaries
      double cx = Xneg+x*(Xpos-Xneg)/(wx-1);
      double cy = Yneg+y*(Ypos-Yneg)/(hy-1);
      //Call calculation engine for each cell
      int iter = mandel(cx,cy,maxiter);
      
      //Add up the iteration count
      count+=iter;
      
      //Color the pixel using the 8 standard colors, dependent on the amount of iterations    
      if (iter<=7){ 
        tft.drawPixel(x,y,BLUE);
      }
      else if (iter>7 && iter<=19){
        tft.drawPixel(x,y,RED);
      }
      else if (iter>19 && iter<=27){
        tft.drawPixel(x,y,GREEN);
      }
      else if (iter>27 && iter<=42){
        tft.drawPixel(x,y,CYAN);
      }
      else if (iter>42 && iter<=65){
        tft.drawPixel(x,y,MAGENTA);
      }
      else if (iter>65 && iter<=84){
        tft.drawPixel(x,y,YELLOW);
      }
      else if (iter>84 && iter<=98){
        tft.drawPixel(x,y,WHITE);
      }
      else{
        tft.drawPixel(x,y,BLACK);
      }
    }
  }
  return count; //return the amount of iterations to be displayet later
}

//Startup screen, greets the user and lets him select different pictures
//returns selection as integer
int startscreen(){
  int selection;
  tft.fillScreen(BLACK);
  //Greet User
  tft.setCursor(0,0);
  tft.setTextSize(2);
  tft.print("OZ1LQO Mandelbrot");
  tft.print("\nRev. 1.0 2014.07.13");
  tft.print("\n\nSelect Area");
  
  //Draw Six rectangle pushbuttons for the user to select areas
  tft.drawRect(0,75,239,50,CYAN);
  tft.setCursor(25,90);
  tft.setTextColor(WHITE);  
  tft.setTextSize(2);
  tft.print("Basic Mandel");
  
  tft.drawRect(0,130,239,50,CYAN);
  tft.setCursor(25,145);
  tft.setTextColor(WHITE);  
  tft.setTextSize(2);
  tft.print("SeaHorse");
  
  tft.drawRect(0,185,239,50,CYAN);
  tft.setCursor(25,200);
  tft.setTextColor(WHITE);  
  tft.setTextSize(2);
  tft.print("Elephant");
  
  tft.drawRect(0,240,239,50,CYAN);
  tft.setCursor(25,255);
  tft.setTextColor(WHITE);  
  tft.setTextSize(2);
  tft.print("Mini Mandel");
  
  tft.drawRect(0,295,239,50,CYAN);
  tft.setCursor(25,310);
  tft.setTextColor(WHITE);  
  tft.setTextSize(2);
  tft.print("Triple Spiral");
  
  tft.drawRect(0,350,239,50,CYAN);
  tft.setCursor(25,365);
  tft.setTextColor(WHITE);  
  tft.setTextSize(2);
  tft.print("Scepter");
  
  //Stop here and wait for the user to touch the display.
  
  uint8_t flag=0;
  
  while(!flag){
  p=touch.getpos(&flag) ;
  }
  
  
  int ly=p.y;
  //check all 6 push button combinations.
  //It's enough to check just the y-position since the
  //buttons take up the entire display in the x-direction
  
  if (ly>881 && ly<1311){
    selection=1;
  }
  
  else if (ly>1442 && ly<1822){
    selection=2;
  }
  
  else if (ly>1900 && ly<2300){
    selection=3;
  }
  
  else if (ly>2422 && ly<2849){
    selection=4;
  }
  
  else if (ly>3000 && ly<3370){
    selection=5;
  }
  
  else if (ly>3453 && ly<4000){
    selection=6;
  }
  else{
    selection=0; //react upon if the user touches an invalid area
  }
  return selection;
}

//Function that waits for the user to touch the screen
//Used as 'pause' at he different  screens
void userwait(){
  uint8_t flag=0;
  
  while(!flag){
  p=touch.getpos(&flag) ;
  }
}

//main loop
void loop() {

  //Getting tft resolution
  int wx = tft.width();
  int hy = tft.height();
  
  //Get the user selection and initialize time measurement, iterations and max iteration variables
  int choice=startscreen();
  unsigned long time_start;
  int maxiter = 100;
  int iterations;
  //long count;
  
  //Run the selected simulation based on the user input
  switch (choice){
    case 1:
    time_start=millis();
    iterations = mandelplot(-2.2, 0.6,-1.2,1.2,maxiter,wx,hy);//Basic Mandelbrot
    break;
    case 2:
    time_start=millis();
    iterations = mandelplot(-0.82, -0.68,0.04,0.16,maxiter,wx,hy);//Seahorse Valley
    break;
    case 3:
    time_start=millis();
    iterations = mandelplot(0.247, 0.303,-0.024,0.024,maxiter,wx,hy);//Elephant Valley
    break;
    case 4:
    time_start=millis();
    iterations = mandelplot(-1.82, -1.68,-0.06,0.06,maxiter,wx,hy);//Mini Mandel (the one in front of the big one)
    break;
    case 5:
    time_start=millis();
    iterations = mandelplot(-0.116, 0.06,0.63,0.678,maxiter,wx,hy);//Triple spiral Valley
    break; 
    case 6:
    time_start=millis();
    iterations = mandelplot(-1.388, -0.828,-0.01,0.47,maxiter,wx,hy);//Scepter Valley
    break;
    case 0://React if an areay outside the pushbuttons were touched
    time_start=millis();
    iterations = 0;
    tft.fillScreen(BLACK);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.setCursor(0,0);
    tft.println("Invalid selection");
    tft.println("Touch to continue");
    break;
  }
  
    
  unsigned long time_stop;
  //record stop time
  time_stop=millis();
  
  userwait();
  
  
  //print some stats ..
  tft.fillScreen(BLACK);
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.println("Iterations:");
  tft.println(iterations);
  tft.println("\nSeconds:");
  tft.println((time_stop-time_start)/1000);
  tft.println("\nIterations / sec.:");
  tft.println(iterations/((time_stop-time_start)/1000));
  tft.println("\nTouch to return");
  tft.println("\nto main screen");
  
  userwait();

}



