public abstract class Comanda {
    public abstract void executa(Imagine imagine);
    public abstract void anuleaza(Imagine imagine);

    public static void main(String[] args) {
        Editor editor = new Editor();
        Imagine imagine = new Imagine(4, 6, 15);
        editor.adaugaComanda(new ComandaRedimensionare());
        editor.executaComenzi(imagine);
        System.out.println(imagine.toString());
        editor.reexecuta(imagine);
        System.out.println(imagine.toString());

        editor.adaugaComanda(new ComandaDecupare());
        editor.executaComenzi(imagine);
        System.out.println(imagine.toString());

        editor.adaugaComanda(new ComandaLuminozitate());

        editor.executaComenzi(imagine);
        System.out.println(imagine.toString());
        editor.anuleaza(imagine);
        System.out.println(imagine.toString());


    }

}
