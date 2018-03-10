import java.util.Random;

public class Generator {
    public static void main(String... args) {

        final int x = 4;
        final int y = 4;

        int layout[][] = new int[x][y];

        Random rand = new Random();

        int curr_x = rand.nextInt(4);

        int curr_y = y - 1;
        int direction = 0;

        layout[curr_x][curr_y] = 1;

        //System.out.println("Starting with " + curr_x + " " + curr_y);

        while (curr_y >= 0) {


            if (direction == 0) {
                if (curr_y == 0)
                    direction = rand.nextInt(2) + 1;
                else {

                    int remaining_left_tiles = x - curr_x;
                    int remaining_right_tiles = x - remaining_left_tiles;


                    int l_chances = 4;
                    int r_chances = 4;

                    if (curr_x == 3) {
                        l_chances = 25;
                    }
                    if (curr_x == 0) {
                        r_chances = 25;
                    }

                    int r = rand.nextInt(r_chances + l_chances) + 1;

                    if (r < l_chances)
                        direction = 1;
                    else if (r >= l_chances && r <= l_chances + r_chances)
                        direction = 2;
                    else {
                        //System.out.println("direction 3 !!!");
                        direction = 3;
                    }

                }
                //System.out.println("Randomizing direction");
            }

            if (direction == 1) {

                if (curr_x == 0) {
                    direction = 3;
                } else {
                    curr_x--;
                    layout[curr_x][curr_y] = 1;
                    //System.out.println("Direction " + direction + " Setting " + curr_x + "," + curr_y + " as " + 1);
                    for (int b = 0; b < y; b++) {
                        //System.out.print(" " + layout[b][curr_y]);
                    }
                    //System.out.println();
                }
            } else if (direction == 2) {

                if (curr_x == 3) {
                    direction = 3;
                } else {
                    curr_x++;
                    layout[curr_x][curr_y] = 1;
                    //System.out.println("Direction " + direction + " Setting " + curr_x + "," + curr_y + " as " + 1);
                    for (int b = 0; b < y; b++) {
                        //System.out.print(" " + layout[b][curr_y]);
                    }
                    //System.out.println();
                }

            } else if (direction == 3) {

                if (curr_y > 0) {

                    //System.out.println("Direction " + direction + " Setting " + curr_x + "," + curr_y + " as " + 2);
                    layout[curr_x][curr_y] = 2;

                    for (int b = 0; b < y; b++) {
                        //System.out.print(" " + layout[b][curr_y]);
                    }
                    //System.out.println();

                    if (curr_y >= 1) {
                        layout[curr_x][curr_y - 1] = 3;
                        //System.out.println("Direction " + direction + " Setting " + curr_x + "," + (curr_y - 1) + " as " + 3);
                        for (int b = 0; b < y; b++) {
                            //System.out.print(" " + layout[b][curr_y]);
                        }
                        //System.out.println();
                    }

                    direction = 0;
                    curr_y--;
                } else
                    break;

            }
        }

        for (int b = y - 1; b >= 0; b--) {
            for (int a = 0; a < x; a++) {
                System.out.print(" " + layout[a][b]);
            }
            System.out.println('\n');
        }

    }

    // 0 0 2 0
    // 0 0 0 0
    // 0 0 0 0
    // 0 0 0 0

}
