package command;
import pokemon.Pokemon;

public interface Command {
    void execute(Pokemon pokemon);
}
