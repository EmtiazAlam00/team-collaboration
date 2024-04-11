#ifndef MENU_H
#define MENU_H

#include <QString>
#include <QStringList>
#include <QVector>

/* Class Purpose: Represents a navigable menu structure within the application.
 * Allows for the creation of hierarchical menu systems with support for sub-menus
 * and navigation back to parent menus.
 *
 * Data Members:
 * - QString name: The name or title of the menu.
 * - QStringList menuItems: A list of textual representations of the menu items.
 * - QVector<Menu*> subMenus: A vector of pointers to sub-menus, allowing for hierarchical menu structures.
 * - Menu* parent: A pointer to the parent menu, enabling navigation back up the menu hierarchy.
 *
 * Class Functions:
 * - explicit Menu(QString name, QStringList menuItems, Menu* parent): Constructor initializing the menu with a name, list of menu items, and an optional parent menu.
 * - ~Menu(): Destructor that cleans up dynamically allocated sub-menus to prevent memory leaks.
 * - void addChildMenu(Menu* menu): Adds a sub-menu to the current menu.
 * - QString getName(): Returns the name of the menu.
 * - QStringList getMenuItems(): Returns the list of menu items.
 * - Menu* getParent(): Returns a pointer to the parent menu, if any.
 * - Menu* get(int index): Returns a pointer to the sub-menu at the specified index.
 */

class Menu
{
public:
    explicit Menu(QString name, QStringList menuItems, Menu* parent = nullptr);
    ~Menu();

    QString getName();
    QStringList getMenuItems();
    Menu* getParent();
    int getPos();
    Menu* get(int index);
    void addChildMenu(Menu* menu);

private:
    QString name;
    QStringList menuItems;
    QVector<Menu*> subMenus;
    Menu* parent;
};

#endif // MENU_H
