
class intro <T>  {   // class

   private String title;  //private so only accesseabel with in that class 
   private String book;   // these are well instance variables 
   private int year;    // those lines declare the instance variable these variables hold the state/data of the object.



public intro (String title, String book, int year) {   // constructor ( this area of the constructor is the parameters )

   this.title= title;   // these three statements assigning values to those parametets to instance variables of the object being created 
   this.book = book;
   this.year = year;


}

public void read () {
 System.out.println("Jackson read this booked called");
}



public String gettitle () {  //these are get methods 
   return title;
}
public String getbook () { // there used to retrieve the values of private instance variables from outside the class. 
   return book;
}

public int getyear () {  // in this case there just returning the direct variables 
   return year;

}
}



class main {
   public static void main (String[] args) {
    // Create instances of the intro class

    intro book1 = new intro("Goblet of Fire ", "Harry potter", 2011);
    intro book2 = new intro ("F1 Race", "Richard King", 2008);

    //Using the instances
    //this shit alittle weird but its a combonation of the method and the instance your created 
    book1.read();
    book2.read();

      System.out.println("The books is title is " + book1.gettitle() +" and the book name is "+ book1.getbook() + "and its year is "+ book1.getyear());

   }

}

// A class is a blueprint for creating objects. It defines the structure (attributes)
// and behavior (methods) that the objects created from the class will have.

//An instance is a specific object created from a class. 
//Each instance has its own set of attribute values, distinct from other instances.
