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

    def __repr__(self):
        return str(self.time)


class TimeInterval():

    def __init__(self, stime, etime):
        self.stime = stime
        self.etime = etime


class Account():

    def __init__(self, name):
        self.name = name


class Asset():

    def __init__(self, cname):
        self.cname = cname
        self.ctime = datetime.datetime.now()


class Audio(Asset):

    def __init__(self, cname, path):
        Asset.__init__(self, cname)
        self.path = path


class AudioNote(Asset):

    def __init__(self, cname, message):
        Asset.__init__(self, cname)
        self.message = message


class AudioMarker(Asset):

    def __init__(self, cname, timestamp):
        Asset.__init__(self, cname)
        self.timestamp = timestamp
        self.noteList = list()

    def addNote(self, name, message):
        self.noteList.append(AudioNote(name, message))


class AudioDocument(Asset):

    def __init__(self, cname, audio):
        Asset.__init__(self, cname)
        self.audio = audio
        self.markerList = list()

    def addMarker(self, name, timestamp):
        self.markerList.append(AudioMarker(name, timestamp))


class AudioSlices(Asset):

    def __init__(self, cname, audio):
        Asset.__init__(self, cname)
        self.audio = audio
        self.slicesList = list()

    def addSlice(self, stime, etime):
        self.slicesList.append(TimeInterval(stime, etime))


# class AssetACL():

#     def __init__(self, asset):
#         self.asset = asset
#         self.readList = list()
#         self.writeList = list()


# class AccountAssets():

#     def __init__(self, account):
#         self.account = account
#         self.assetList = dict()
        

class Application():

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
        print " *executeMenu"
        command = raw_input("%s@%s: " % (self.account.name if self.account else "anonymous", self.appname))
        return self.menu[command]()

    def help(self):
        print "%s" % ("|".join(self.menu.keys()))

    def start(self):
        print " *start"
        self.running = True

    def execute(self):
        print " *execute"
        self.start()
        while self.isRunning():
            ret = self.executeMenu()
        return ret

    def end(self):
        print " *end"
        self.running = False

    def isRunning(self):
        return (self.running is True)


class BashApplication(Application):

    def __init__(self, account):
        Application.__init__(self, account)
        self.appname = "Bash"
        self.runningApp = None
        if account is None:
            self.addMenu("login", self.login)
        else:
            self.addMenu("logout", self.logout)
            self.addMenu("recorder", self.recorder)
            self.addMenu("slicer", self.slicer)
            self.addMenu("notetaker", self.notetaker)

    def execute(self):
        ret = 0
        self.start()
        print " *execute: ", self.isRunning()
        while self.isRunning() is True:
            print " *running application: %s" % (self.runningApp)
            if self.runningApp:
                ret = self.runningApp.execute()
                self.runningApp = None
            else:
                ret = Application.executeMenu(self)
        return ret

    def login(self):
        print " *login"
        name = raw_input("Account: ")
        account = Account(name)
        self.account = account
        self.removeMenu("login")
        self.addMenu("logout", self.logout)
        self.addMenu("recorder", self.recorder)
        self.addMenu("slicer", self.slicer)
        self.addMenu("notetaker", self.notetaker)
        return 0

    def logout(self):
        print " *logout"
        self.account = None
        self.removeMenu("logout")
        self.removeMenu("recorder")
        self.removeMenu("slicer")
        self.removeMenu("notetaker")
        self.addMenu("login", self.logout)
        return 0

    def recorder(self):
        print " *create recorder"
        self.runningApp = RecorderApplication(self.account)
        return 0

    def slicer(self):
        print " *create slicer"
        self.runningApp = SlicerApplication(self.account)
        return 0

    def notetaker(self):
        print " *create notetaker"
        self.runningApp = NoteTakerApplication(self.account)
        return 0


class RecorderApplication(Application):

    CHUNK = 1024
    FORMAT = pyaudio.paInt16
    CHANNELS = 2
    RATE = 44100

    def __init__(self, account):
        Application.__init__(self, account)
        self.appname = "Recorder"
        self.addMenu("record", self.record)
        self.recording = False
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
        if self.recording == False and self.recordingThread == None:
            self.recording = True
            self.recordingThread = Thread(target=self.recording, name="recording thread")
            self.recordingThread.start()
            self.addMenu("stop", self.stop)
            return 0
        return 1

    def recording(self):
        print("* recording")
        p = pyaudio.PyAudio()
        stream = p.open(format=self.format,
                        channels=self.channels,
                        rate=self.rate,
                        input=True,
                        frames_per_buffer=self.chunk)
        self.frames = []

        while self.recording == True:
            data = stream.read(self.chunk)
            self.frames.append(data)

        print("* done recording")
        stream.stop_stream()
        stream.close()

        p.terminate()
        self.sampwidth = p.get_sample_size(self.format)

        self.addMenu("save", self.save)

    def getTiming(self):
        return numpy.divide(len(self.frames) * self.chunk, self.rate, dtype=numpy.float64)

    def stop(self):
        if self.recording == True and self.recordingThread:
            self.recording = False
            self.recordingThread.join()
            self.recordingThread = None

    def save(self):
        filename = raw_input("wav filename: ")
        filepath = "/Users/ChiYuChen/UCSC extension/Object Oriented Analysis and Design/%s/%s" % (self.account.name, filename)
        wf = wave.open(filepath, 'wb')
        wf.setnchannels(self.channels)
        wf.setsampwidth(self.sampwidth)
        wf.setframerate(self.rate)
        wf.writeframes(b''.join(self.frames))
        wf.close()


class AudioDocumentRecorder(RecorderApplication):

    def __init__(self, account):
        RecorderApplication.__init__(self, account)
        self.appname = "AudioDocumentRecorder"
        self.audiodoc = AudioDocument(self.account.name, None)
        self.noteList = list()
        self.addMenu("record", self.record)

    def record(self):
        if RecorderApplication.record(self) == 0:
            self.addMenu("mark", self.mark)
            self.addMenu("stop", self.stop)
            return 0
        return 1

    def mark(self):
        self.audiodoc.addMarker(self.account.name, self.getTiming())
        return 0

    def stop(self):
        if RecorderApplication.stop(self) == 0:
            
            return 1
        return 0

    def save(self):
        filename = raw_input("audiodocument filename: ")
        filepath = "/Users/ChiYuChen/UCSC extension/Object Oriented Analysis and Design/%s/%s" % (self.account.name, filename)
        with open(filepath, "w+") as f:
            pickle.dump(self.audiodoc, f)
        return 0

class SlicerApplication(Application):

    def __init__(self, account):
        Application.__init__(self, account)
        self.appname = "Slicer"
        self.addMenu("slice", self.slice)
        self.audio = None
        self.audioslices = None

    def slice(self):
        print " *slice"
        audioName = raw_input("Audio filename: ")
        audioPath = "/Users/ChiYuChen/UCSC extension/Object Oriented Analysis and Design/%s/%s" % (self.account.name, audioName)
        self.audio = Audio(audioPath)
        [Fs, x] = aIO.readAudioFile(audioPath)
        segments = aS.getEventDetection(x, Fs, 0.020, 0.020, smoothWindow = 0.27, weight = 0.3, plot = False)
        self.audioslices = AudioSlices(self.audio)
        for segment in segments:
            self.audioslices.addSlice(segment[0], segment[1])
        self.addMenu("save", self.save)
        return 0

    def save(self):
        audioslicesName = raw_input("Audioslices filename: ")
        audioslicesPath = "/Users/ChiYuChen/UCSC extension/Object Oriented Analysis and Design/%s/%s" % (self.account.name, audioslicesName)
        with open(audioslicesPath, "w+") as f:
            pickle.dump(self.audioslices, f)
        return 0


class NoteTakerApplication(Application):

    CHUNK = 1024

    def __init__(self, account):
        Application.__init__(self, account)
        self.appname = "NoteTakerApplication"
        self.addMenu("load", self.load)
        self.audioslices = None
        self.audioslicesIdx = 0
        self.start

    def load(self):
        audioslicesName = raw_input("Audioslices filename: ")
        audioslicesPath = "/Users/ChiYuChen/UCSC extension/Object Oriented Analysis and Design/%s/%s" % (self.account.name, audioslicesName)
        with open(audioslicesPath, "r") as f:
            self.audioslices = pickle.load(f)
            self.addMenu("play", self.play)
            self.addMenu("next", self.next)
            self.addMenu("prev", self.prev)
            self.addMenu("repeat", self.repeat)
            return 0
        return 1

    def play(self):
        wf = wave.open(self.audioslices.audio.name, 'rb')
        timeInterval = self.audioslices.slicesList[self.audioslicesIdx]

        p = pyaudio.PyAudio()
        channels = wf.getframerate()
        rate = wf.getframerate()
        sampwidth = wf.getsampwidth()
        stream = p.open(format=p.get_format_from_width(sampwidth),
                        channels=channels,
                        rate=rate,
                        output=True)

        print "getframerate: %d\n", wf.getframerate()
        print "getnframes: %d\n", wf.getnframes()
        print "tell: %d\n", wf.tell()

        data = wf.readframes(NoteTakerApplication.CHUNK)
        print "tell: %d\n", wf.tell()

        while data != '':
            stream.write(data)
            data = wf.readframes(NoteTakerApplication.CHUNK)
            print "tell: %d\n", wf.tell()

        stream.stop_stream()
        stream.close()

        p.terminate()

    def next(self):
        self.audioslicesIdx = self.audioslicesIdx + 1

    def prev(self):
        self.audioslicesIdx = self.audioslicesIdx - 1

    def repeat(self):
        self.play()


def main3():
    import pyaudio
    import wave
    import sys

    CHUNK = 1024

    wf = wave.open("output.wav", 'rb')

    p = pyaudio.PyAudio()

    stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
                    channels=wf.getnchannels(),
                    rate=wf.getframerate(),
                    output=True)

    print("getframerate: %d\n", wf.getframerate())
    print("getnframes: %d\n", wf.getnframes())
    print("tell: %d\n", wf.tell())
    data = wf.readframes(CHUNK)
    print("tell: %d\n", wf.tell())
    while data != '':
        stream.write(data)
        data = wf.readframes(CHUNK)
        print("tell: %d\n", wf.tell())

    stream.stop_stream()
    stream.close()

    p.terminate()

def record():
    command = ""
    recorder = SpeechNoteRecorder()
    recorder.start()
    while command != "S":
        command = raw_input("Next command [N/S]: ")
        if command == "":
            timing = recorder.getTiming()
            bookmark = TimeStamp(timing)
            recorder.addBookmark(bookmark)
            print "Note at %f!" % timing
    recorder.stop()
    recorder.save("output2.wav")
    print recorder

def main5():
    bashapp = BashApplication(None)
    result = bashapp.execute()

if __name__ == "__main__":
    main5()