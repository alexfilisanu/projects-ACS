package pokemon;

public class Ability {
    private int damage;
    private boolean stun;
    private boolean dodge;
    private int cooldown;

    public Ability(int damage, boolean stun, boolean dodge, int cooldown) {
        this.damage = damage;
        this.stun = stun;
        this.dodge = dodge;
        this.cooldown = cooldown;
    }

    public int getDamage() { return damage; }
    public void setDamage(int damage) { this.damage = damage; }
    public boolean isStun() { return stun; }
    public void setStun(boolean stun) { this.stun = stun; }
    public boolean isDodge() { return dodge; }
    public void setDodge(boolean dodge) { this.dodge = dodge; }
    public int getCooldown() { return cooldown; }
    public void setCooldown(int cooldown) { this.cooldown = cooldown; }

    @Override
    public String toString() {
        return "pokemon.Ability{" +
                "damage=" + damage +
                ", stun=" + stun +
                ", dodge=" + dodge +
                ", cooldown=" + cooldown +
                '}';
    }
}
