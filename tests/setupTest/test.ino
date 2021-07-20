#line 2 "basic.ino"

// Adapted from:
// https://github.com/mmurdoch/arduinounit/blob/master/examples/basic/basic.ino

#include <AUnit.h>


/*
Setup code
*/
int idx = 0;
for(int i = 1; i<4; i++){
  for(int j = 1; j<4; j++){
    for(int k = 1; k<4; k++){
      cols[idx].c = alpha[idx];
      cols[idx].color[0] = i;
      cols[idx].color[1] = j;
      if(k == 3){
        cols[idx].color[2] = k+1;
      } else { cols[idx].color[2] = k;}
      idx++;
    }
  }
}

/*
Test case 1
*/
test(correct) {
  int A[3] = {1,1,1};
  int B[3] = {1,1,2};
  int C[3] = {1,1,4};
  int D[3] = {1,2,1};
  int E[3] = {1,2,2};
  int F[3] = {1,2,4};
  int G[3] = {1,4,1};
  int H[3] = {1,4,2};
  int I[3] = {1,4,4};
  int J[3] = {2,1,1};
  int K[3] = {2,1,2};
  int L[3] = {2,1,4};
  int M[3] = {2,2,1};
  int N[3] = {2,2,2};
  int O[3] = {2,2,4};
  int P[3] = {2,4,1};
  int Q[3] = {2,4,2};
  int R[3] = {2,4,4};
  int S[3] = {4,1,1};
  int T[3] = {4,1,2};
  int U[3] = {4,1,4};
  int V[3] = {4,2,1};
  int W[3] = {4,2,2};
  int X[3] = {4,2,4};
  int Y[3] = {4,4,1};
  int Z[3] = {4,4,2};
  int space[3] = {4,4,4};

  assertEqual(x[3], );
  assertEqual();
}

/*
Test case 2
*/
test(incorrect) {
  int x = 1;
  assertNotEqual(x, 1);
}

//----------------------------------------------------------------------------
// setup() and loop()
//----------------------------------------------------------------------------

void setup() {
  delay(1000); // wait for stability on some boards to prevent garbage Serial
  Serial.begin(115200); // ESP8266 default of 74880 not supported on Linux
  while(!Serial); // for the Arduino Leonardo/Micro only

  Serial.println(F("This test should produce the following:"));
  Serial.println(
    F("1 passed, 1 failed, 0 skipped, 0 timed out, out of 2 test(s).")
  );
  Serial.println(F("----"));
}

void loop() {
  aunit::TestRunner::run();
}
