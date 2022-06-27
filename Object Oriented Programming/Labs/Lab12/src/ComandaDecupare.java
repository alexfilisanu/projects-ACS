public class ComandaDecupare extends Comanda{
    private int verificare;

    @Override
    public void executa(Imagine imagine) {
        if (imagine.getLatime() > 20)
            imagine.setLatime(imagine.getLatime() - 20);
        else return;
        if (imagine.getLungime() > 30)
            imagine.setLungime(imagine.getLungime() - 30);
        else {
            imagine.setLatime(imagine.getLatime() + 20);
            return;
        }
        this.verificare++;
    }

    @Override
    public void anuleaza(Imagine imagine) {
        if (this.verificare != 0) {
            imagine.setLatime(imagine.getLatime() + 20);
            imagine.setLungime(imagine.getLungime() + 30);
            this.verificare--;
        }
    }
}
