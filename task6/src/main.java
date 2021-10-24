import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.concurrent.locks.Lock;

import static java.lang.Thread.sleep;

public class main {

    static int count = 0;
    static long end = 0;
    static int countTimes = 1000000;
    static int N = 20; // 线程数从1到N
    static int M = 1;
    public static void main(String[] args) throws InterruptedException {

        long[] BackoffLockTimeCosts = new long[N];
        long[] TASLockTimeCosts = new long[N];
        long[] TTASLockTimeCosts = new long[N];
//        long[] AndersonLockTimeCosts = new long[N];
        long[] CLHLockTimeCosts = new long[N];
        for(int j = 0; j < M; j++) {

            for(int i = 1; i < N; i+=2) {
                System.out.println("当前进程数:"+i);
                Lock lock = new BackoffLock();
                long timeCost = getTimeCost(lock, N);
                BackoffLockTimeCosts[i] += timeCost;


                lock = new TASLock();
                timeCost = getTimeCost(lock, N);
                TASLockTimeCosts[i] += timeCost;


                lock = new TTASLock();
                timeCost = getTimeCost(lock, N);

                TTASLockTimeCosts[i] += timeCost;

//            lock = new AndersonLock(N);
//            timeCost = getTimeCost(lock, N);
//            writeTxt(timeCost, "TTASLock.txt");
//            AndersonLockTimeCosts[i] = timeCost;

                lock = new TTASLock();
                timeCost = getTimeCost(lock, N);
                CLHLockTimeCosts[i] += timeCost;
            }
        }


        for(int i = 1; i < N; i+=2) {
            writeTxt(TTASLockTimeCosts[i]/M,"TTASLockTimeCosts.txt");
            writeTxt(TASLockTimeCosts[i]/M,"TASLockTimeCosts.txt");
            writeTxt(BackoffLockTimeCosts[i]/M,"BackoffLockTimeCosts.txt");
//            writeTxt(AndersonLockTimeCosts[i],"AndersonLockTimeCosts.txt");
            writeTxt(CLHLockTimeCosts[i]/M,"CLHLockTimeCosts.txt");
        }


        System.out.println(count);
    }


    private static long getTimeCost(Lock lock, int N) throws InterruptedException {
        long timeCost = 0;

        count = 0;
        long start = System.currentTimeMillis();


        for(int i = 0; i < N; i++) {
            int times = countTimes / N;
            if (i == N - 1) {
                times += countTimes % N;
            }
            int finalTimes = times;
            new Thread(() -> runMethod(lock, finalTimes), "thread").start();
        }

        while(true) {
            sleep(1);
            if(count == countTimes) {
                end = System.currentTimeMillis();
                timeCost = (end - start);
                break;
            }

        }
        return timeCost;
    }


    private static void runMethod(Lock lock, int times) {
        for(int i = 0; i < times; i++) {
            lock.lock();
            count++;
            lock.unlock();
        }
    }

    private static void writeTxt(long writeData, String filename) {
        FileWriter fw = null;
        try {
            //如果文件存在，则追加内容；如果文件不存在，则创建文件
            File f=new File(filename);
            if(!f.exists()) {
                f.createNewFile();
            }
            fw = new FileWriter(f, true);
        } catch (IOException e) {
            e.printStackTrace();
        }
        PrintWriter pw = new PrintWriter(fw);
        pw.println(writeData);
        pw.flush();
        try {
            fw.flush();
            pw.close();
            fw.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}



