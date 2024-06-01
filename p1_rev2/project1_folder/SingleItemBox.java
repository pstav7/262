/**
 * A class representing a box that can store a single item of any type.
 * 
 * @param <T> the type of the item stored in the box
 */
public class SingleItemBox<T> {
    private T item;

    /**
     * Constructs a SingleItemBox with the specified item.
     * 
     * @param item the item to be stored in the box
     */
    public SingleItemBox(T item) {
        this.item = item;
    }

    /**
     * Retrieves the item stored in the box.
     * 
     * @return the item stored in the box
     */
    public T getItem() {
        return item;
    }
}
