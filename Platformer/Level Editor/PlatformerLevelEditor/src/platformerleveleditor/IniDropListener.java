/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package platformerleveleditor;

import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.dnd.DnDConstants;
import java.awt.dnd.DropTargetDragEvent;
import java.awt.dnd.DropTargetDropEvent;
import java.awt.dnd.DropTargetEvent;
import java.awt.dnd.DropTargetListener;
import java.io.File;
import java.util.List;

/**
 *
 * @author Michael Jordan (14868336)
 */
public abstract class IniDropListener implements DropTargetListener{

    @Override
    public void dragEnter(DropTargetDragEvent dtde) {
        //Do Nothing
    }

    @Override
    public void dragOver(DropTargetDragEvent dtde) {
        //Do Nothing
    }

    @Override
    public void dropActionChanged(DropTargetDragEvent dtde) {
        //Do Nothing
    }

    @Override
    public void dragExit(DropTargetEvent dte) {
        //Do Nothing
    }

    @Override
    public void drop(DropTargetDropEvent event) {
        // Accept copy drops
        event.acceptDrop(DnDConstants.ACTION_COPY);

        // Get the transfer which can provide the dropped item data
        Transferable transferable = event.getTransferable();

        // Get the data formats of the dropped item
        DataFlavor[] flavors = transferable.getTransferDataFlavors();

        // Loop through the flavors
        for (DataFlavor flavor : flavors) {

            try {

                // If the drop items are files
                if (flavor.isFlavorJavaFileListType()) {

                    // Get all of the dropped files
                    List<File> files = (List) transferable.getTransferData(flavor);

                    // Loop them through
                    for (File file : files) 
                    {
                        String name = file.getName();
                        String ext = "";
                        int i = name.lastIndexOf('.');
                        if(i > 0 && i < name.length() - 1)
                        {
                            ext = name.substring(i+1).toLowerCase();
                        }
                        if(ext == "ini")
                        {
                            acceptDrop(file);
                        }

                    }

                }

            } catch (Exception e) {

                // Print out the error stack
                e.printStackTrace();

            }
        }
    }
    
    public abstract void acceptDrop(File file); 
    
}
