#! c:/python27/python.exe
import sys, time

class ProgressBar:
    def __init__(self, count = 0, total = 0, width = 50):
        self.count = count
        self.total = total
        self.width = width
        
    def move(self):
        self.count += 1
        
    def log(self, s):
        progress = self.width * self.count / self.total
        sys.stdout.write(' {0:3}/{1:3}: '.format(self.count, self.total))
        sys.stdout.write('>' * progress + '=' * (self.width - progress) + '\r')
        if progress == self.width:
            sys.stdout.write('\n')
        sys.stdout.flush()
        
    def end(self):
        self.count = self.total
        self.log('')

def main():
    print ("Hello Python!")
    
    bar = ProgressBar(total = 100)
    
    for line in range(1, 100):
        bar.move()
        bar.log('')
        time.sleep(0.01)

if __name__ == "__main__":
    main()


