import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Tablou {
    private final static int CAPACITATE = 8;
    private LinkedList<Integer> list = new LinkedList<>();
    private Lock lock = new ReentrantLock();
    private Condition neVid = lock.newCondition();
    private Condition nePlin = lock.newCondition();

    public static int getCapacitate() { return CAPACITATE; }
    public LinkedList<Integer> getList() { return list; }

    void offer(int valoare) {
        list.add(valoare);
    }

    void scrie(int valoare) {
        lock.lock();
        try {
            while (list.size() == CAPACITATE)
                nePlin.await();
            offer(valoare);
            neVid.signal();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            lock.unlock();
        }
    }

    int citeste() {
        lock.lock();
        int valoare = 0;
        try {
            while (list.size() == 0)
                neVid.await();
            valoare = list.remove();
            nePlin.signal();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            lock.unlock();
        }
        return valoare;
    }
}
