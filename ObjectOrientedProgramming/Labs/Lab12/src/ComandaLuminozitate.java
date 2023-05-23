public class ComandaLuminozitate extends Comanda{
    private int verificare;

    @Override
    public void executa(Imagine imagine) {
        imagine.setNivelLuminozitate((int)(imagine.getNivelLuminozitate() * 1.5));
        this.verificare++;
    }

    @Override
    public void anuleaza(Imagine imagine) {
        if (this.verificare != 0) {
            imagine.setNivelLuminozitate((int) (imagine.getNivelLuminozitate() * (2 / 3)));
            this.verificare--;
        }
    }
}
