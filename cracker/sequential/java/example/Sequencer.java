package sequencer;

/**
 *
 * @author sanguita
 */
public class Sequencer {
    
    private static final char[] CHARSET = "0123456789".toCharArray();
    private static final int PASSWORD_LENGTH = 4;
    
    public static void main(String[] args) {
        int charset_length = CHARSET.length;
        double key_space = Math.pow(charset_length, PASSWORD_LENGTH);
        System.out.println("Key space is:\t"+key_space);
        int idx;
        System.out.println("Initializing indexes...");
        int idxArr[] = new int[PASSWORD_LENGTH];
        for(idx=0; idx < PASSWORD_LENGTH; idx++){
            idxArr[idx] = 0;
        }
        System.out.println("Generating...");
        for(idx=0; idx<key_space;idx++){
            int base = idx;
            for(int div=0; div < PASSWORD_LENGTH; div++){
                idxArr[PASSWORD_LENGTH-div-1] = base%charset_length;
                base = base/charset_length;
            }
            for(int i=0; i<idxArr.length;i++){
                System.out.print(CHARSET[idxArr[i]]);
            }
            System.out.println();
        }
        System.out.println("DONE");
    }
}
