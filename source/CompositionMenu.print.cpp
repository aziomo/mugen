void CompositionMenu::startPlayback(){
  musicBox->stopPlaying();
  musicBox->playbackKeys = false;
  musicBox->startPlaying();
  musicBox->writeThread =
	thread(&CompositionMenu::playbackTimeline, this);
}

void CompositionMenu::playbackTimeline(){
  double globalTime = musicBox->globalTime;
  double beginTime = globalTime;
  double timeBetweenCols = 60.0 / timeline->tempo;
  double timeElapsed = 0.0;
  double lastColTriggerTime = -timeBetweenCols;
  double songLength = timeline->segColumns() 
	  * timeBetweenCols * timeline->songSegs.size();

  while (timeElapsed < songLength){
    std::unique_lock<std::mutex> mu(musicBox->mu_blocksReadyToRead);
    if (musicBox->blocksReadyToOutput == musicBox->maxBlockCount){
      musicBox->cv_blocksReadyToWrite.wait(mu);
    }
    if (!musicBox->isRunning) break;
    if (timeElapsed - lastColTriggerTime > timeBetweenCols){
      int colsElapsed =
		(int)(timeElapsed / timeBetweenCols) % timeline->segColumns();
      int segsElapsed =
		timeElapsed / timeBetweenCols / timeline->segColumns();
      playCurrentNotes(globalTime, colsElapsed, segsElapsed);
      releasePrevNotes(globalTime, colsElapsed, segsElapsed);
      lastColTriggerTime = timeElapsed;
    }
    removeSilentNotes();
    if (musicBox->blocksReadyToOutput < musicBox->maxBlockCount){
      musicBox->writeBitsToBuffer(&bitsPlayed);
    }
    globalTime = musicBox->globalTime;
    timeElapsed = globalTime - beginTime;
  }
  bitsPlayed.clear();
}
