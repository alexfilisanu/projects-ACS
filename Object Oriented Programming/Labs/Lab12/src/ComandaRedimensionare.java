public class ComandaRedimensionare extends Comanda{
    private int verificare;

    @Override
    public void executa(Imagine imagine) {
        imagine.setLatime((int)(imagine.getLatime() * 1.5));
        imagine.setLungime((int)(imagine.getLungime() * 1.5));
        this.verificare++;
    }

    @Override
    public void anuleaza(Imagine imagine) {
        if (this.verificare != 0) {
            imagine.setLatime((int) (imagine.getLatime() * (2 / 3)));
            imagine.setLungime((int) (imagine.getLungime() * (2 / 3)));
            this.verificare--;
        }
    }
}
