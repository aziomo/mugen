void startPlaying() {
  isRunning = true;
  readThread = thread(&MusicBox::outputLoop, this);
  if (keysPlayback){
    writeThread = thread(&MusicBox::inputLoop, this);
  }
}

void writePressedKeysToBuffer() {
  if (blocksReadyToOutput < maxBlockCount) {
    unique_lock<mutex> locker(mu_blocksReadyToRead);
    if (blocksReadyToOutput == 0)
      cv_blocksReadyToWrite.wait(locker);
    if (!isRunning){
      return;
    }
    bool anyPlaying = false;
    for (auto note: pressedNotes)
      if (note.isAudible) {
        anyPlaying = true;
        break;
      }

    if (anyPlaying) {
      float *newBlock = new float[blockSize];
      zeroOutArray(newBlock, blockSize);
      for (int i = 0; i < KEYBOARD_SIZE; i++) {
        if (pressedNotes[i].isAudible) {
          instruments.at(currentInstrument)
            ->addToBlock(newBlock, &pressedNotes[i], gTime);
        }
      }
      gTime += timeStep * blockSize;
      blocksBuffer.push(newBlock);

      blocksReadyToOutput++;
      if (blocksReadyToOutput == 1)
        cv_blocksReadyToRead.notify_one();
    }
  }
}
/**/
void writeBitsToBuffer(vector<Bit*> *bits){
  auto *newBlock = new float[blockSize];
  zeroOutArray(newBlock, blockSize);
  for (auto bit : *bits) {
    bit->instrument->addToBlock(newBlock, &bit->note, gTime);
  }
  gTime += timeStep * blockSize;
  blocksBuffer.push(newBlock);
  blocksReadyToOutput++;
  if (blocksReadyToOutput == 1){
    cv_blocksReadyToRead.notify_one();
  }
}

void copyBlock(const float *source, float *destination) const {
  for (int i = 0; i < blockSize; i++) {
    destination[i] = source[i];
  }
}

void inputLoop() {
  while (isRunning) {
    writePressedKeysToBuffer();
  }
}

void outputLoop() {
  float outputBlock[blockSize];
  while (isRunning) {
    if (!readBlockFromBuffer(outputBlock)) break;
    if (outputFile != nullptr) 
      writeBlockToFile(outputBlock);
    else
      audioApi->writeOut(outputBlock);
  }
}

bool readBlockFromBuffer(float *outputBlock) {
  unique_lock<mutex> locker(mu_blocksReadyToRead);
  if (blocksReadyToOutput == 0)
    cv_blocksReadyToRead.wait(locker);
  if (!isRunning)
    return false;

  copyBlock(blocksBuffer.front(), outputBlock);
  blocksBuffer.pop();
  blocksReadyToOutput--;
  if (blocksReadyToOutput == maxBlockCount - 1){
    cv_blocksReadyToWrite.notify_one();
  }
  return true;
}

