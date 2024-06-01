/**
 *  This is a demo of using the SingleItemBox class.
 *  @author K. Raven Russell
 */
public class BoxUsageDemo {
	/**
	 *  This is a main method with demo code.
	 *  @param args command line args (not used)
	 */
	public static void main(String[] args) {
		//demo putting an apple in a box
		
		/**
		 * Class for apples.
		 */
		class Apple { }
		
		//make an apple
		Apple a1 = new Apple();
		
		//put the apple in a box
		SingleItemBox<Apple> appleBox = new SingleItemBox<>(a1);
		
		//check that the apple was put in the box
		if(appleBox.getItem().equals(a1)) {
			System.out.println("yay 1");
		}
		
		
		
		//demo putting a banana in a box
		
		/**
		 * Class for bananas.
		 */
		class Banana { }
		
		//make a banana
		Banana b1 = new Banana();
		
		//put the banana in a box
		SingleItemBox<Banana> bananaBox = new SingleItemBox<>(b1);
		
		//check that the banana was put in the box
		if(bananaBox.getItem().equals(b1)) {
			System.out.println("yay 2");
		}
		
		
		
		//demo putting a banana in a box
		
		/**
		 * Class for cats.
		 */
		class Cat { }
		
		//make a banana
		Cat c1 = new Cat();
		
		//put the banana in a box
		SingleItemBox<Cat> catPlayBox = new SingleItemBox<>(c1);
		
		//check that the banana was put in the box
		if(catPlayBox.getItem().equals(c1)) {
			System.out.println("yay 3");
		}
	}
}