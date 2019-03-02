package penis;

public class Main(){
  public static void main(String[] args) {
    Car car = new Car(5, "Volvo");
    Car car2 = new Car(9, "Mercedes");

    Car car3 = car2;

    System.out.println(car.brand);




    ...
    ...


    car = null;
  }


}

final class Car(){
  public final int capacity = 20;
  protected String brand;
  private int pocet;

  public Car(int capacity, String brand){
    this.capacity = capacity;

    this.brand = brand;
  }

  public final void countOfPassengers(){
    ...
  }
}

class Autobus extends Car(){

}
