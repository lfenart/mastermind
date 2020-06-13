/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package lfenart.mastermind;

import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author lfenart
 */
public class Grid {
    
    public static final int MAX_SIZE = 10;
    
    private List<Combination> lines;
    
    public Grid() {
        this.lines = new ArrayList<>();
    }
    
    public List<Combination> getLines() {
        return this.lines;
    }
    
    public Combination getLine(int index) {
        return this.lines.get(index);
    }
    
    public void play(Combination c) {
        this.lines.add(c);
    }
}
