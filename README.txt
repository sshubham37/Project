/*
This program includes Customer billing which manages inventory for a small grocery shop owner.
Copyright (C) 2016 Shubham Singh sshubham37@ymail.com

 *This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
*/

------------------------------------------------ Customer Billing Project---------------------------------------------------------------------------------
DSA Project

Name: Shubham Singh  
MIS: 111408056
Branch: Information Technology

The following Software is just a mini version for buying products and printing bills for a small grocery shop. The interface is built using ncurses.
In the following software we can view Inventory which is actually the food items we have added in "data.txt" file.

It has entries in following manner:

<Item name, %s> <quantity, %d> <cost, %f>
where %s, %d and %f are scanf format specifiers. Item name does not have spaces. 
We can edit the data from "data.txt" file.

After viewing Inventory there is an option of "New Bill". When we press Enter, We can add new items in Bill from the inventory. We can add items by pressing enter on the food item we want. Also, there is an option of Search items on top where we can search items that we have entered or search items from inventory.  After adding all required food items, when we checkout the software calculates the total amount and prints the total bill at the left bottom corner. Then we can Exit.

View Inventory before selecting new items.
Use Arrow keys(Up and Down) to Navigate while viewing inventory and adding new items. Press Enter to Select any option. 

/* Compile : Type "make". Then type "./project". */
--------------------------------------------------------------------------------------------------------------------------------------------------------------------- 
