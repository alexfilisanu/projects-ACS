package pokemon;

public class PokemonBuilder {
    private Pokemon pokemon = new Pokemon();

    public Pokemon build() {
        return pokemon;
    }

    public PokemonBuilder withName(String name) {
        pokemon.setName(name);
        return this;
    }

    public PokemonBuilder withHp(int hp) {
        pokemon.setHp(hp);
        return this;
    }

    public PokemonBuilder withAttack(int attack) {
        pokemon.setAttack(attack);
        return this;
    }

    public PokemonBuilder withSpecialAttack(int specialAttack) {
        pokemon.setSpecialAttack(specialAttack);
        return this;
    }

    public PokemonBuilder withDefense(int defense) {
        pokemon.setDefense(defense);
        return this;
    }

    public PokemonBuilder withSpecialDefense(int specialDefense) {
        pokemon.setSpecialDefense(specialDefense);
        return this;
    }
    public PokemonBuilder withFirstAbility(Ability ability) {
        pokemon.setFirstAbility(ability);
        return this;
    }

    public PokemonBuilder withSecondAbility(Ability ability) {
        pokemon.setSecondAbility(ability);
        return this;
    }
}
