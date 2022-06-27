public class Arena {
    private int busy = 0;

    public Arena() {
        this.busy = 0;
    }

    public synchronized void enterArena() {
        while (busy >= 1) {
            try {
                wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        busy++;
    }

    public synchronized void leaveArena(){
        busy--;
        notifyAll();
    }
}
