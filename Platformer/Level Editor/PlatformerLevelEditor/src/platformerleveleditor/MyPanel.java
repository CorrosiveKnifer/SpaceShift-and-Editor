/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package platformerleveleditor;
import java.awt.BorderLayout;
import javax.swing.JPanel;
import javax.swing.BorderFactory;
import java.awt.Color;
import java.awt.Graphics;
/**
 *
 * @author Michael Jordan (14868336)
 */
public class MyPanel extends JPanel{
   
    final class rect{
        int x, y;
        int w, h;
    } 
   
    public MyPanel() {
        setBorder(BorderFactory.createLineBorder(Color.black));
        setLayout(new BorderLayout());
    }
    
    
    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);       

        // Draw Text
        g.drawString("This is my custom Panel!",10,20);
        g.drawRect(10, 10, 100, 100);
    }  
}
