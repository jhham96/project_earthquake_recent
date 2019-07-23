//Sensor Data saving into RAM
void sensor_checking() {

  int channel=3;//three channel
  int numSamples=5000;//25*200 (25 sec * 200 Hz)
  float *sd[channel]; //three sensor channel

  for (i = 0; i < channel; ++i) {
    sd[i] = (float *)sdcalloc(numSamples, sizeof (float));
    assert(sd[i]);
  }

  for (i = 0; i < numSamples;) {
    adxl345_readDataPt(temp) ;//read 3-axis data// temp 센서에서 계측한 데이터 3축 데이터 (every instance) <이건 예시임>

    for (j = 0; j < channel; j++) {
      sd[j][i] = (float) temp[j];
    }
  }

  free(sd);//free the memory after data processing

}
