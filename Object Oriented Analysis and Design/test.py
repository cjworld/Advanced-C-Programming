import datetime
# For recording and playing
from threading import Thread
import pyaudio
import wave
import numpy
import pickle
# For splitting
import os
from pyAudioAnalysis import audioBasicIO as aIO
from pyAudioAnalysis import audioSegmentation as aS


class TimeStamp():

    def __init__(self, time):
        self.time = time


class TimeInterval():

    def __init__(self, stime, etime):
        self.stime = stime
        self.etime = etime

    def __str__(self):
        return "(%s, %s)" % (self.stime, self.etime)

    def __repr__(self):
        return "(%s, %s)" % (self.stime, self.etime)


class Account():

    def __init__(self, name):
        self.name = name


class Metadata():

    def __init__(self, cname):
        self.cname = cname
        self.ctime = datetime.datetime.now()


class Audio(Metadata):

    def __init__(self, cname, path):
        Metadata.__init__(self, cname)
        self.path = path

    def __str__(self):
        return "%s" % (self.path)

    def __repr__(self):
        return "%s" % (self.path)


class SplittedAudio(Metadata):

    def __init__(self, cname, audio, sentenceList):
        Metadata.__init__(self, cname)
        self.audio = audio
        self.sentenceList = sentenceList

    def __str__(self):
        return "%s: %s" % (self.audio, self.sentenceList)

    def __repr__(self):
        return "%s: %s" % (self.audio, self.sentenceList)


class Mark(Metadata):

    def __init__(self, cname, timestamp):
        Metadata.__init__(self, cname)
        self.timestamp = timestamp

    def __str__(self):
        return "%f" % (self.timestamp)

    def __repr__(self):
        return "%f" % (self.timestamp)


class AudioMarkList(Metadata):

    def __init__(self, cname, audio, markList):
        Metadata.__init__(self, cname)
        self.audio = audio
        self.markList = markList

    def __str__(self):
        return "%s: %s" % (self.audio, self.markList)

    def __repr__(self):
        return "%s: %s" % (self.audio, self.markList)


class Application(object):

    def __init__(self, account):
        self.running = False
        self.account = account
        self.appname = "Application"
        self.menu = dict()
        self.addMenu("help", self.help)
        self.addMenu("end", self.end)

    def addMenu(self, name, func):
        if name not in self.menu:
            self.menu[name] = func

    def removeMenu(self, name):
        if name in self.menu:
            del self.menu[name]

    def executeMenu(self):
        while True:
            command = raw_input("%s@%s: " % (self.account.name if self.account else "anonymous", self.appname))
            if command in self.menu:
                break
            print "Unknown command."
        return self.menu[command]()

    def help(self):
        print "%s" % ("|".join(self.menu.keys()))

    def start(self):
        self.running = True

    def execute(self):
        self.start()
        while self.isRunning():
            ret = self.executeMenu()
        return ret

    def end(self):
        self.running = False

    def isRunning(self):
        return (self.running is True)


class BashApplication(Application):

    def __init__(self, account):
        Application.__init__(self, account)
        self.appname = "Bash"
        self.runningApp = None
        self.loginAppList = [("logout", self.logout), ("markingRecorder", self.markingRecorder), ("markingPlayer", self.markingPlayer), ("audioSplitter", self.audioSplitter), ("splittedAudioPlayer", self.splittedAudioPlayer), ("audioMarkListPlayer", self.audioMarkListPlayer)]
        self.logoutAppList = [("login", self.login)]
        if account is None:
            self.changeState(False)
        else:
            self.changeState(True)

    def execute(self):
        ret = 0
        self.start()
        while self.isRunning() is True:
            if self.runningApp:
                ret = self.runningApp.execute()
                self.runningApp = None
            else:
                ret = Application.executeMenu(self)
        return ret

    def changeState(self, logined):
        if logined:
            for (appName, appHandler) in self.logoutAppList:
                self.removeMenu(appName)
            for (appName, appHandler) in self.loginAppList:
                self.addMenu(appName, appHandler)
        else:
            for (appName, appHandler) in self.loginAppList:
                self.removeMenu(appName)
            for (appName, appHandler) in self.logoutAppList:
                self.addMenu(appName, appHandler)

    def login(self):
        name = raw_input("Account: ")
        account = Account(name)
        self.account = account
        self.changeState(True)
        return 0

    def logout(self):
        self.account = None
        self.changeState(False)
        return 0

    def markingRecorder(self):
        self.runningApp = MarkingRecorderApp(self.account)
        return 0

    def markingPlayer(self):
        self.runningApp = MarkingPlayerApp(self.account)
        return 0

    def audioSplitter(self):
        self.runningApp = AudioSplitterApp(self.account)
        return 0

    def splittedAudioPlayer(self):
        self.runningApp = SplittedAudioPlayerApp(self.account)
        return 0

    def audioMarkListPlayer(self):
        self.runningApp = AudioMarkListPlayer(self.account)
        return 0


class Recorder():

    CHUNK = 1024
    FORMAT = pyaudio.paInt16
    CHANNELS = 2
    RATE = 44100

    def __init__(self):
        self.isRecording = False
        self.MAXSECONDS = 60 * 60 * 5
        self.channels = Recorder.CHANNELS
        self.rate = Recorder.RATE
        self.format = Recorder.FORMAT
        self.chunk = Recorder.CHUNK
        self.sampwidth = 0
        self.stream = None
        self.frames = []
        self.recordingThread = None

    def record(self):
        if self.isRecording == False and self.recordingThread == None:
            self.isRecording = True
            self.recordingThread = Thread(target=self.recording, name="recording thread")
            self.recordingThread.start()
            return 0
        return 1

    def recording(self):
        p = pyaudio.PyAudio()
        stream = p.open(format=self.format,
                        channels=self.channels,
                        rate=self.rate,
                        input=True,
                        frames_per_buffer=self.chunk)
        self.frames = []

        while self.isRecording == True:
            data = stream.read(self.chunk)
            self.frames.append(data)

        stream.stop_stream()
        stream.close()
        p.terminate()
        self.sampwidth = p.get_sample_size(self.format)

    def stop(self):
        if self.isRecording == True and self.recordingThread:
            self.isRecording = False
            self.recordingThread.join()
            self.recordingThread = None
            return 0
        return 1

    def save(self, cname, path):
        audio = Audio(cname, path)
        wf = wave.open(path, 'wb')
        wf.setnchannels(self.channels)
        wf.setsampwidth(self.sampwidth)
        wf.setframerate(self.rate)
        wf.writeframes(b''.join(self.frames))
        wf.close()
        return audio


class Marker():

    def __init__(self):
        self.markList = list()

    def mark(self, cname, timestamp):
        mark = Mark(cname, timestamp)
        self.markList.append(mark)

    def reset(self):
        self.markList = list()

    def save(self, cname, audio, path):
        audioMarkList = AudioMarkList(cname, audio, self.markList)
        with open(path, "w+") as f:
            pickle.dump(audioMarkList, f)
        return audioMarkList


class MarkingRecorderApp(Recorder, Marker, Application):

    def __init__(self, account):
        Application.__init__(self, account)
        Marker.__init__(self)
        Recorder.__init__(self)
        self.appname = "MarkingRecorderApp"
        self.addMenu("record", self.record)

    def record(self):
        if Recorder.record(self) == 0:
            self.markList = list()
            self.removeMenu("record")
            self.addMenu("stop", self.stop)
            self.addMenu("mark", self.mark)
            return 0
        return 1

    def stop(self):
        if Recorder.stop(self) == 0:
            self.removeMenu("stop")
            self.removeMenu("mark")
            self.addMenu("record", self.record)
            self.addMenu("save", self.save)
            return 0
        return 1

    def mark(self):
        timestamp = numpy.divide(len(self.frames) * self.chunk, self.rate, dtype=numpy.float64)
        print "* mark at %f" % (timestamp)
        Marker.mark(self, self.account.name, timestamp)

    def save(self):
        recorderName = raw_input("Recorder filename: ")
        recorderPath = "/Users/ChiYuChen/UCSC extension/Object Oriented Analysis and Design/%s/%s.wav" % (self.account.name, recorderName)
        audio = Recorder.save(self, self.account.name, recorderPath)
        
        amlName = raw_input("AudioMarkList filename: ")
        amlPath = "/Users/ChiYuChen/UCSC extension/Object Oriented Analysis and Design/%s/%s.aml" % (self.account.name, amlName)
        Marker.save(self, self.account.name, audio, amlPath)

    def end(self):
        self.stop()
        Application.end(self)


class Player():
    
    CHUNK = 1024
    CHANNELS = 2
    RATE = 44100

    def __init__(self):
        self.audio = None
        self.stime = 0
        self.etime = None
        self.frames = []
        self.channels = Player.CHANNELS
        self.rate = Player.RATE
        self.chunk = Player.CHUNK
        self.sampwidth = 0
        self.isPlaying = False
        self.playingThread = None

    def load(self, account, filepath):
        self.audio = Audio(account.name, filepath)
        return 0

    def play(self):
        if self.audio != None and self.isPlaying == False and self.playingThread == None:
            self.isPlaying = True
            self.playingThread = Thread(target=self.playing, name="playing thread")
            self.playingThread.start()
            return 0
        return 1

    def playing(self):
        wf = wave.open(self.audio.path, 'rb')
        p = pyaudio.PyAudio()
        self.channels = wf.getnchannels()
        self.rate = wf.getframerate()
        self.sampwidth = wf.getsampwidth()
        stream = p.open(format=p.get_format_from_width(self.sampwidth),
                        channels=self.channels,
                        rate=self.rate,
                        output=True)


        spos = self.stime * self.rate
        if self.etime:
            epos = self.etime * self.rate
        wf.setpos(spos)

        self.frames = []
        data = wf.readframes(self.chunk)
        while data != '' and self.isPlaying == True:
            stream.write(data)
            self.frames.append(data)
            if self.etime and epos < wf.tell():
                break
            data = wf.readframes(self.chunk)

        stream.stop_stream()
        stream.close()
        p.terminate()

    def stop(self):
        if self.isPlaying == True and self.playingThread:
            self.isPlaying = False
            self.playingThread.join()
            self.playingThread = None
            return 0
        return 1


class MarkingPlayerApp(Player, Marker, Application):

    def __init__(self, account):
        Application.__init__(self, account)
        Marker.__init__(self)
        Player.__init__(self)
        self.appname = "MarkingPlayerApp"
        self.addMenu("load", self.load)

    def load(self):
        filename = raw_input("Audio filename: ")
        filepath = "/Users/ChiYuChen/UCSC extension/Object Oriented Analysis and Design/%s/%s" % (self.account.name, filename)
        if Player.load(self, self.account, filepath) == 0:
            self.removeMenu("load")
            self.addMenu("play", self.play)
            return 0
        return 1

    def play(self):
        if Player.play(self) == 0:
            Marker.reset(self)
            self.removeMenu("play")
            self.addMenu("stop", self.stop)
            self.addMenu("mark", self.mark)
            return 0
        return 1

    def stop(self):
        if Player.stop(self) == 0:
            self.removeMenu("stop")
            self.removeMenu("mark")
            self.addMenu("play", self.play)
            self.addMenu("save", self.save)
            return 0
        return 1

    def mark(self):
        timestamp = numpy.divide(len(self.frames) * self.chunk, self.rate, dtype=numpy.float64)
        print "* mark at %f" % (timestamp)
        Marker.mark(self, self.account.name, timestamp)

    def save(self):
        amlName = raw_input("AudioMarkList filename: ")
        amlPath = "/Users/ChiYuChen/UCSC extension/Object Oriented Analysis and Design/%s/%s.aml" % (self.account.name, amlName)
        Marker.save(self, self.account.name, self.audio, amlPath)

    def end(self):
        self.stop()
        Application.end(self)


class AudioSplitterApp(Application):

    def __init__(self, account):
        Application.__init__(self, account)
        self.appname = "AudioSplitterApp"
        self.addMenu("load", self.load)
        self.audio = None
        self.sentenceList = list()

    def load(self):
        filename = raw_input("Audio filename: ")
        filepath = "/Users/ChiYuChen/UCSC extension/Object Oriented Analysis and Design/%s/%s" % (self.account.name, filename)
        self.audio = Audio(self.account.name, filepath)
        self.removeMenu("load")
        self.addMenu("split", self.split)
        return 0

    def split(self):
        [Fs, x] = aIO.readAudioFile(self.audio.path)
        segments = aS.getEventDetection(x, Fs, 0.020, 0.020, smoothWindow = 0.27, weight = 0.3, plot = False)
        for segment in segments:
            self.sentenceList.append(TimeInterval(segment[0], segment[1]))
        self.removeMenu("split")
        self.addMenu("save", self.save)
        return 0

    def save(self):
        splittedAudioName = raw_input("Splitted audio filename: ")
        splittedAudioPath = "/Users/ChiYuChen/UCSC extension/Object Oriented Analysis and Design/%s/%s.sa" % (self.account.name, splittedAudioName)
        splittedAudio = SplittedAudio(self.account.name, self.audio, self.sentenceList)
        with open(splittedAudioPath, "w+") as f:
            pickle.dump(splittedAudio, f)
        return 0


class SplittedAudioPlayerApp(Player, Application):

    def __init__(self, account):
        Application.__init__(self, account)
        Player.__init__(self)
        self.appname = "SplittedAudioPlayerApp"
        self.addMenu("load", self.load)
        self.splittedAudio = None
        self.sentenceIdx = 0

    def load(self):
        splittedAudioName = raw_input("Splitted Audio filename: ")
        splittedAudioPath = "/Users/ChiYuChen/UCSC extension/Object Oriented Analysis and Design/%s/%s" % (self.account.name, splittedAudioName)
        with open(splittedAudioPath, "r") as f:
            self.splittedAudio = pickle.load(f)
            if Player.load(self, self.account, self.splittedAudio.audio.path) == 0:
                self.removeMenu("load")
                self.addMenu("play", self.play)
                self.addMenu("stop", self.stop)
                self.addMenu("next", self.next)
                self.sentenceIdx = 0
                return 0
        return 1

    def play(self):
        self.stime = self.splittedAudio.sentenceList[self.sentenceIdx].stime
        self.etime = self.splittedAudio.sentenceList[self.sentenceIdx].etime
        self.stop()
        if Player.play(self) == 0:
            return 0
        return 1

    def next(self):
        self.addMenu("prev", self.prev)
        if self.sentenceIdx < len(self.splittedAudio.sentenceList) - 1:
            self.sentenceIdx = self.sentenceIdx + 1
            if self.sentenceIdx == len(self.splittedAudio.sentenceList) - 1:
                self.removeMenu("next")
            self.play()

    def prev(self):
        self.addMenu("next", self.next)
        if self.sentenceIdx > 0:
            self.sentenceIdx = self.sentenceIdx - 1
            if self.sentenceIdx == 0:
                self.removeMenu("prev")
            self.play()

    def end(self):
        self.stop()
        Application.end(self)


class AudioMarkListPlayer(SplittedAudioPlayerApp):

    def __init__(self, account):
        SplittedAudioPlayerApp.__init__(self, account)
        self.appname = "AudioMarkListPlayer"
        self.addMenu("load", self.load)
        self.aml = None
        self.markIdx = 0

    def load(self):
        if SplittedAudioPlayerApp.load(self) == 0:
            amlName = raw_input("AudioMarkList filename: ")
            amlPath = "/Users/ChiYuChen/UCSC extension/Object Oriented Analysis and Design/%s/%s" % (self.account.name, amlName)
            with open(amlPath, "r") as f:
                self.aml = pickle.load(f)
                print "aml: ", self.aml
                self.removeMenu("load")
                self.addMenu("nextMark", self.nextMark)
                self.firstMark()
                return 0
        return 1

    def findSentenceForMark(self, mark):
        for sentenceIdx, sentence in enumerate(self.splittedAudio.sentenceList):
            if sentence.stime <= mark.timestamp and mark.timestamp <= sentence.etime:
                return sentenceIdx
        return -1

    def firstMark(self):
        sentenceIdx = self.findSentenceForMark(self.aml.markList[0])
        if sentenceIdx:
            self.sentenceIdx = sentenceIdx

    def nextMark(self):
        self.addMenu("prevMark", self.prev)
        if self.markIdx < len(self.aml.markList) - 1:
            self.markIdx = self.markIdx + 1
            if self.markIdx == len(self.aml.markList) - 1:
                self.removeMenu("nextMark")
            sentenceIdx = self.findSentenceForMark(self.aml.markList[self.markIdx])
            if sentenceIdx:
                self.sentenceIdx = sentenceIdx
                self.play()

    def prevMark(self):
        self.addMenu("nextMark", self.nextMark)
        if self.markIdx > 0:
            self.markIdx = self.markIdx - 1
            if self.markIdx == 0:
                self.removeMenu("prevMark")
            sentenceIdx = self.findSentenceForMark(self.aml.markList[self.markIdx])
            if sentenceIdx:
                self.sentenceIdx = sentenceIdx
                self.play()


def main():
    bashapp = BashApplication(None)
    result = bashapp.execute()

if __name__ == "__main__":
    main()