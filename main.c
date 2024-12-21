#include "foundation.h"

#include "utils.h"
#include "db.h"
#include "config.h"
#include "user.h"
#include "console.h"
#include "visual.h"
#include "gps.h"
#include "datetime.h"
#include "random.h"

// store location
#define GPS_STORE_LOC_LAT -7.558619412433269
#define GPS_STORE_LOC_LON 110.85807559904933

// tables
db_Table *t_users;
db_Table *t_carts;
db_Table *t_products;
db_Table *t_address;
db_Table *t_orders;
db_Table *t_bankAccounts;
db_Table *t_userPayments;
db_Table *t_agents;
db_Table *t_orderPackages;
db_Table *t_returnRequest;

char *path_users = "db/user_accounts.txt";
char *path_carts = "db/user_carts.txt";
char *path_products = "db/products.txt";
char *path_address = "db/user_address.txt";
char *path_orders = "db/orders.txt";
char *path_bankAccounts = "db/bank_accounts.txt";
char *path_userPayments = "db/user_payments.txt";
char *path_agents = "db/agents.txt";
char *path_orderPackages = "db/order_packages.txt";
char *path_returnRequest = "db/return_request.txt";

char *path_dbConfig = "config/admin.txt";

// user
User *u_activeUser;

// configs
Config *db_conf;

// views
void view_menu();

void view_login();
void view_signup();
void view_logout();

void view_search();
void view_productPage(db_Row *r_product);
void view_productCardList(db_Row r_product);

void view_cart();
void view_cartEditItems();

void view_orders();

void view_checkout();
void view_checkoutEditAddress(int *p_address);
void view_checkoutEditPayment(int *p_bankAccount);
void view_checkoutEditAgent(int *p_agent);

void view_settings();
void view_settingsAddAddress();
void view_settingsEditAddress();
void view_settingsDeleteAddress();
void view_settingsConnectBank();
void view_settingsDisconnectBank();

void view_adminManageProducts();
void view_adminManageOrders();

int main()
{
    // Initialize everything
    t_users = db_fileToTable(path_users, 2);
    t_carts = db_fileToTable(path_carts, 3);
    t_products = db_fileToTable(path_products, 7);
    t_address = db_fileToTable(path_address, 5);
    t_orders = db_fileToTable(path_orders, 10);
    t_bankAccounts = db_fileToTable(path_bankAccounts, 5);
    t_userPayments = db_fileToTable(path_userPayments, 2);
    t_agents = db_fileToTable(path_agents, 4);
    t_orderPackages = db_fileToTable(path_orderPackages, 3);
    t_returnRequest = db_fileToTable(path_returnRequest, 1);

    // configuration for database
    db_conf = conf_newConfig();
    conf_addFromFile(db_conf, "config/admin.txt");

    // db_printTable(*t_products);
    // db_printTable(*t_orders);
    // con_anyKey();

    // active user recorder
    u_activeUser = usr_newUser();

    // call the menu view
    view_menu();

    // save tables when exiting
    db_saveTable(*t_users, path_users);
    db_saveTable(*t_carts, path_carts);
    db_saveTable(*t_products, path_products);
    db_saveTable(*t_address, path_address);
    db_saveTable(*t_orders, path_orders);
    db_saveTable(*t_bankAccounts, path_bankAccounts);
    db_saveTable(*t_userPayments, path_userPayments);
    db_saveTable(*t_agents, path_agents);
    db_saveTable(*t_orderPackages, path_orderPackages);
    db_saveTable(*t_returnRequest, path_returnRequest);
}

void view_menu()
{
    // input
    int n_input;

    while (true)
    {
        con_clearScr();

        /*
            different mode different title
        */

        if (u_activeUser->is_login)
        {
            if (u_activeUser->is_seller)
                (void)printf("%sWelcome to Seller Center%s\nLogged in as: %s%s%s!\n", FG_YELLOW, FG_DEFAULT, FG_GREEN, u_activeUser->username, FG_DEFAULT);
            else
            {
                (void)printf("Welcome, %s%s%s!\n", FG_GREEN, u_activeUser->username, FG_DEFAULT);
            }
        }
        else
            (void)printf("%sNot logged in!%s\n", FG_RED, FG_DEFAULT);

        vis_printBars(V_BAR, con_getSize()->x);

        /*
            when user is logged in
            - if customer shows the customer menu
            - if seller show the seller menu
        */
        if (u_activeUser->is_login)
        {
            // if seller
            if (u_activeUser->is_seller)
                vis_printListMenu(4, "Search for products", "Manage products", "Manage orders", "Log out");
            // if customer
            else
                vis_printListMenu(5, "Search for products", "My cart", "My orders", "Account settings", "Log out");
        }
        /*
            when user is not logged in
            show the menu with login and signup option
        */
        else
            vis_printListMenu(4, "Login to an existing account", "Create new account", "Search for products", "Exit");

        vis_printBars(V_BAR, con_getSize()->x);

        /*
            INPUT
            based on mode of course
        */
        con_printColor("Selection: ", FG_PROMPT);
        n_input = con_inputInt();

        // if you're login
        if (u_activeUser->is_login)
        {
            // if you're seller
            if (u_activeUser->is_seller)
            {
                switch (n_input)
                {
                case 1:
                    // admin view search
                    view_search();
                    continue;
                case 2:
                    // admin manage products
                    view_adminManageProducts();
                    continue;
                case 3:
                    // admin manage orders

                    continue;
                case 4:
                    // admin log out
                    view_logout();
                    continue;
                default:
                    continue;
                }
            }
            // if you're customer
            else
            {
                switch (n_input)
                {
                case 1:
                    view_search();
                    continue;
                case 2:
                    view_cart();
                    continue;
                case 3:
                    view_orders();
                    continue;
                case 4:
                    view_settings();
                    continue;
                case 5:
                    view_logout();
                    continue;
                default:
                    continue;
                }
            }
        }
        // if you're not logged in
        else
        {
            switch (n_input)
            {
            case 1:
                view_login();
                continue;
            case 2:
                view_signup();
                continue;
            case 3:
                view_search();
                continue;
            case 4:
                break;
            default:
                continue;
            }
        }

        /*
            by default this will break or exit the loop
            because we're using switch case to check the input
            so if you want to continue the input, use continue at case
        */
        break;
    }
}

void view_login()
{
    if (u_activeUser->is_login)
        return;

    int n_input;
    char *s_usnInput = (char *)malloc(sizeof(char) * 255);
    char *s_pwInput = (char *)malloc(sizeof(char) * 255);

    while (true)
    {
        con_clearScr();

        (void)printf("Login as?\n");
        vis_printBars(V_BAR, con_getSize()->x);
        vis_printListMenu(3, "Customer", "Seller", "Exit");
        vis_printBars(V_BAR, con_getSize()->x);

        con_printColor("Selection: ", FG_PROMPT);
        n_input = con_inputInt();

        switch (n_input)
        {
        case 1:
            // Login as a customer
            con_clearScr();

            (void)printf("Login as customer\n");
            vis_printBars(V_BAR, con_getSize()->x);

            con_printColor("Username: ", FG_PROMPT);
            s_usnInput = con_inputStr();

            db_Row *login_row = db_selectFirstRowWhere(*t_users, db_getColIdx(*t_users, "username"), s_usnInput);
            if (!login_row)
            {
                con_printColor("Username did not exist! (Press Any Key) . . .", FG_ERROR);
                con_anyKey();
                continue;
            }

            con_printColor("Password: ", FG_PROMPT);
            s_pwInput = con_inputPwd();

            if (strcmp(login_row->elements[db_getColIdx(*t_users, "password")], s_pwInput) != 0)
            {
                con_printColor("Wrong password! (Press Any Key) . . .", FG_ERROR);
                con_anyKey();
                continue;
            }

            u_activeUser->username = strdup(s_usnInput);
            u_activeUser->is_seller = false;
            u_activeUser->is_login = true;

            break;
        case 2:
            // Login as a seller
            con_clearScr();

            (void)printf("Login as administrator\n");
            vis_printBars(V_BAR, con_getSize()->x);

            con_printColor("Enter admin username: ", FG_PROMPT);
            s_usnInput = con_inputStr();

            Var *usn_var = conf_getVar(*db_conf, "db_admin_username");
            if (!usn_var)
            {
                con_printColor("Can't find definition of 'db_admin_username' . . .", FG_ERROR);
                con_anyKey();
                continue;
            }

            if (strcmp(s_usnInput, usn_var->val) != 0)
            {
                con_printColor("Wrong admin username!", FG_ERROR);
                con_anyKey();
                continue;
            }

            con_printColor("Password: ", FG_PROMPT);
            s_pwInput = con_inputPwd();

            Var *pw_var = conf_getVar(*db_conf, "db_admin_password");
            if (!pw_var)
            {
                con_printColor("Can't find definition of 'db_admin_password' . . .", FG_ERROR);
                con_anyKey();
                continue;
            }

            if (strcmp(s_pwInput, pw_var->val) != 0)
            {
                con_printColor("Wrong admin password!", FG_ERROR);
                con_anyKey();
                continue;
            }

            u_activeUser->username = strdup(usn_var->val);
            u_activeUser->is_seller = true;
            u_activeUser->is_login = true;

            break;
        case 3:
            break;
        default:
            continue;
        }

        break;
    }
}

void view_signup()
{
    con_clearScr();

    if (u_activeUser->is_login)
        return;

    int n_input;
    char *s_usnInput = (char *)malloc(sizeof(char) * 255);
    char *s_pwInput = (char *)malloc(sizeof(char) * 255);

    (void)printf("Create new account\n");
    vis_printBars(V_BAR, con_getSize()->x);

    con_printColor("New username: ", FG_PROMPT);
    s_usnInput = con_inputStr();

    if (db_selectFirstRowWhere(*t_users, db_getColIdx(*t_users, "username"), s_usnInput))
    {
        con_printColor("Username already exist! (Press Any Key) . . .", FG_ERROR);
        con_anyKey();
        return;
    }

    con_printColor("Password: ", FG_PROMPT);
    s_pwInput = con_inputPwd();

    db_Row *signup_row = db_newRow(t_users->col_count);
    signup_row->id = db_getHighestId(*t_users) + 1;
    signup_row->elements[db_getColIdx(*t_users, "username")] = strdup(s_usnInput);
    signup_row->elements[db_getColIdx(*t_users, "password")] = strdup(s_pwInput);

    db_insertRow(t_users, *signup_row);
    db_saveTable(*t_users, path_users);

    con_printColor("Account successfuly created! (Press Any Key) . . .", FG_GREEN);
    con_anyKey();
}

void view_logout()
{
    if (!u_activeUser->is_login)
        return;

    u_activeUser->is_login = false;
    u_activeUser->is_seller = false;
    u_activeUser->username = strdup("");
}

void view_search()
{
    int n_input;

    char *s_productInput = (char *)malloc(sizeof(char) * 255);
    char *s_categoryInput = (char *)malloc(sizeof(char) * 255);
    char *s_priceStartInput = (char *)malloc(sizeof(char) * 255);
    char *s_priceEndInput = (char *)malloc(sizeof(char) * 255);

    int n_baseIdx;
    int n_showMany;

    bool b_sort_ascending = true;
    bool b_sort_alphabetical = true;
    bool b_minmax_price = false;

    while (true)
    {
        con_clearScr();

        // default values
        strcpy(s_productInput, "");
        strcpy(s_categoryInput, "");
        strcpy(s_priceStartInput, "");
        strcpy(s_priceEndInput, "");
        b_sort_ascending = true;
        b_sort_alphabetical = true;
        b_minmax_price = false;
        n_baseIdx = db_getColIdx(*t_products, "name");
        // end default values

        (void)printf("How do you want to search? ");
        if (!u_activeUser->is_login)
            con_printColor("(You are not logged in)", FG_ERROR);
        (void)printf("\n");

        vis_printBars(V_BAR, con_getSize()->x);
        vis_printListMenu(3, "Simple search", "Advanced search", "Exit");
        vis_printBars(V_BAR, con_getSize()->x);

        con_printColor("Selection: ", FG_PROMPT);
        n_input = con_inputInt();

        switch (n_input)
        {
        case 1:
            // Simple Search
            con_clearScr();
            (void)printf("Enter keyword for the product you want to search\n");
            vis_printBars(V_BAR, con_getSize()->x);

            con_printColor("Enter keyword (press enter to show all products): ", FG_PROMPT);
            s_productInput = con_inputStr();

            break;

        case 2:
            // Advanced Search
            con_clearScr();
            (void)printf("Advanced Search\n");
            vis_printBars(V_BAR, con_getSize()->x);

            con_printColor("Enter keyword (press enter to skip): ", FG_PROMPT);
            s_productInput = con_inputStr();

            con_printColor("Enter category (press enter to skip): ", FG_PROMPT);
            s_categoryInput = con_inputStr();

            vis_printBars(V_BAR, con_getSize()->x);
            printf("How do you want to sort the product list?\n");
            vis_printBars(V_BAR, con_getSize()->x);
            vis_printListMenu(4, "Sort by name (default)", "Sort by price", "Sort by stock", "Sort by numbers sold");
            vis_printBars(V_BAR, con_getSize()->x);

            con_printColor("Selection: ", FG_PROMPT);
            n_input = con_inputInt();

            switch (n_input)
            {
            case 1:
                n_baseIdx = db_getColIdx(*t_products, "name");
                b_sort_alphabetical = true;
                break;
            case 2:
                n_baseIdx = db_getColIdx(*t_products, "price");
                b_sort_alphabetical = false;
                break;
            case 3:
                n_baseIdx = db_getColIdx(*t_products, "stock");
                b_sort_alphabetical = false;
                break;
            case 4:
                n_baseIdx = db_getColIdx(*t_products, "numbers_sold");
                b_sort_alphabetical = false;
                break;
            default:
                n_baseIdx = db_getColIdx(*t_products, "name");
                b_sort_alphabetical = true;
                break;
            }

            vis_printBars(V_BAR, con_getSize()->x);
            printf("Ascending or descending?\n");
            vis_printBars(V_BAR, con_getSize()->x);
            vis_printListMenu(2, "Ascending (default)", "Descending");
            vis_printBars(V_BAR, con_getSize()->x);

            con_printColor("Selection: ", FG_PROMPT);
            n_input = con_inputInt();
            switch (n_input)
            {
            case 1:
                b_sort_ascending = true;
                break;
            case 2:
                b_sort_ascending = false;
                break;
            default:
                b_sort_ascending = true;
            }

            vis_printBars(V_BAR, con_getSize()->x);
            printf("Do you want to set minimum/maximum price?\n");
            vis_printBars(V_BAR, con_getSize()->x);
            vis_printListMenu(2, "Yes", "No (default)");
            vis_printBars(V_BAR, con_getSize()->x);

            con_printColor("Selection: ", FG_PROMPT);
            n_input = con_inputInt();

            if (n_input == 1)
            {
                b_minmax_price = false;
                vis_printBars(V_BAR, con_getSize()->x);

                con_printColor("Minimum price (enter to skip): Rp", FG_PROMPT);
                s_priceStartInput = con_inputStr();

                con_printColor("Maximum price (enter to skip): Rp", FG_PROMPT);
                s_priceEndInput = con_inputStr();

                if (strlen(s_priceStartInput) <= 0 && strlen(s_priceEndInput) <= 0)
                    b_minmax_price = false;
            }
            else
                b_minmax_price = false;

            vis_printBars(V_BAR, con_getSize()->x);
            con_printColor("How many products you want to show? (press enter to show all): ", FG_PROMPT);
            n_showMany = con_inputInt();

            break;
        case 3:
            return; // exit search
        default:
            continue;
        }

        // Show product
        while (true)
        {
            con_clearScr();
            printf("Search result\n");
            vis_printBars(V_BAR, con_getSize()->x);

            db_Row *search_result_row = db_getSortedRows(*t_products, n_baseIdx, b_sort_alphabetical, b_sort_ascending);

            int n_found = 0;

            for (int i = 0; i < t_products->row_count; i++)
            {
                if (!search_result_row[i].is_null)
                {
                    if (strlen(s_productInput) > 0)
                    {
                        if (!(util_isSubstr(util_lowerStr(search_result_row[i].elements[db_getColIdx(*t_products, "name")]), util_lowerStr(s_productInput)) || util_isSubstr(util_lowerStr(search_result_row[i].elements[db_getColIdx(*t_products, "description")]), util_lowerStr(s_productInput))))
                            continue;

                        if (strlen(s_categoryInput) > 0)
                            if (!util_isSubstr(util_lowerStr(search_result_row[i].elements[db_getColIdx(*t_products, "category")]), util_lowerStr(s_categoryInput)))
                                continue;
                    }

                    if (strlen(s_categoryInput) > 0)
                        if (!util_isSubstr(util_lowerStr(search_result_row[i].elements[db_getColIdx(*t_products, "category")]), util_lowerStr(s_categoryInput)))
                            continue;

                    if (strlen(s_priceStartInput) > 0)
                        if (atoi(search_result_row[i].elements[db_getColIdx(*t_products, "price")]) < atoi(s_priceStartInput))
                            continue;

                    if (strlen(s_priceEndInput) > 0)
                        if (atoi(search_result_row[i].elements[db_getColIdx(*t_products, "price")]) > atoi(s_priceEndInput))
                            continue;

                    if (n_showMany > 0 && n_found >= n_showMany)
                        break;

                    // view product
                    printf("\n%s[ID:%d]%s"
                           " - %s[%s]"
                           "\n%s%s%s"
                           "\n%s"
                           "\n%sSold: %s | Stock: %s"
                           "\n%sRp%s%s\n",
                           FG_MAGENTA, search_result_row[i].id, FG_DEFAULT,
                           FG_YELLOW, search_result_row[i].elements[db_getColIdx(*t_products, "category")],
                           FG_CYAN, search_result_row[i].elements[db_getColIdx(*t_products, "name")], FG_DEFAULT,
                           search_result_row[i].elements[db_getColIdx(*t_products, "description")],
                           FG_BLUE, search_result_row[i].elements[db_getColIdx(*t_products, "numbers_sold")], search_result_row[i].elements[db_getColIdx(*t_products, "stock")],
                           FG_GREEN, search_result_row[i].elements[db_getColIdx(*t_products, "price")], FG_DEFAULT);

                    n_found++;
                }
            }

            vis_printBars(V_BAR, con_getSize()->x);
            vis_printListMenu(2, "Select product by ID", "Search again / Back to search menu");
            vis_printBars(V_BAR, con_getSize()->x);

            con_printColor("Selection: ", FG_PROMPT);
            n_input = con_inputInt();

            db_Row *product_row;

            switch (n_input)
            {
            case 1:

                while (true)
                {
                    con_printColor("Enter product Id: ", FG_PROMPT);
                    n_input = con_inputInt();

                    product_row = db_selectRowWhereId(*t_products, n_input);
                    if (product_row == NULL)
                    {
                        con_printColor("Product with that id is not found! (Press Any Key) . . .", FG_ERROR);
                        con_anyKey();
                        continue;
                    }
                    break;
                }

                view_productPage(db_selectRowWhereId(*t_products, n_input));
                continue;
            case 2:
                break;
            default:
                continue;
            }

            break;
        }

        continue;
    }
}

void view_productPage(db_Row *r_product)
{
    int n_input;

    while (true)
    {
        con_clearScr();

        (void)printf("Product details\n");
        vis_printBars(V_BAR, con_getSize()->x);

        printf("\n%s%s%s\n", FG_CYAN, r_product->elements[db_getColIdx(*t_products, "name")], FG_DEFAULT);
        printf("%sRp%s%s\n", FG_GREEN, r_product->elements[db_getColIdx(*t_products, "price")], FG_DEFAULT);

        vis_printBars(V_BAR, con_getSize()->x);

        con_printColor("ID:\n", FG_GREEN);
        printf("%d\n", r_product->id);
        con_printColor("Category:\n", FG_GREEN);
        printf("%s\n", r_product->elements[db_getColIdx(*t_products, "category")]);
        con_printColor("Description:\n", FG_GREEN);
        printf("%s\n", r_product->elements[db_getColIdx(*t_products, "description")]);
        con_printColor("Stock:\n", FG_GREEN);
        printf("%s\n", r_product->elements[db_getColIdx(*t_products, "stock")]);
        con_printColor("Numbers sold:\n", FG_GREEN);
        printf("%s\n", r_product->elements[db_getColIdx(*t_products, "numbers_sold")]);
        con_printColor("Weight:\n", FG_GREEN);
        printf("%s gr\n", r_product->elements[db_getColIdx(*t_products, "weight")]);

        vis_printBars(V_BAR, con_getSize()->x);

        if (u_activeUser->is_login)
        {
            if (u_activeUser->is_seller)
                // TODO:
                vis_printListMenu(1, "Back to search");
            else
                vis_printListMenu(2, "Add product to cart", "Search another product / go back");
        }
        else
        {
            con_printColor("You must login to buy this product!\n", FG_ERROR);
            vis_printListMenu(1, "Search another product / go back");
        }

        vis_printBars(V_BAR, con_getSize()->x);
        con_printColor("Selection: ", FG_PROMPT);
        n_input = con_inputInt();

        if (u_activeUser->is_login)
        {
            if (u_activeUser->is_seller)
            {
                // TODO:
                switch (n_input)
                {
                case 1:
                    break;
                default:
                    continue;
                }
            }
            else
            {
                switch (n_input)
                {
                    // Add Product to Cart
                case 1:
                    while (true)
                    {
                        con_printColor("Enter quantity: ", FG_PROMPT);
                        n_input = con_inputInt();

                        if (n_input > atoi(r_product->elements[db_getColIdx(*t_products, "stock")]))
                        {
                            con_printColor("Nah.. you can't add more than the stock buddy (Press Any Key) . . .", FG_ERROR);
                            con_anyKey();
                            continue;
                        }
                        else if (n_input <= 0)
                        {
                            con_printColor("Please add 1 or more (Press Any Key) . . .", FG_ERROR);
                            con_anyKey();
                            continue;
                        }

                        db_Row *cart_row = db_newRow(t_carts->col_count);
                        cart_row->id = db_getHighestId(*t_carts) + 1;
                        cart_row->elements[db_getColIdx(*t_carts, "username")] = strdup(u_activeUser->username);
                        cart_row->elements[db_getColIdx(*t_carts, "product_id")] = util_intToStr(r_product->id);
                        cart_row->elements[db_getColIdx(*t_carts, "quantity")] = util_intToStr(n_input);

                        db_insertRow(t_carts, *cart_row);

                        // Update the table value
                        r_product->elements[db_getColIdx(*t_products, "stock")] = util_intToStr(
                            atoi(r_product->elements[db_getColIdx(*t_products, "stock")]) -
                            n_input);

                        db_saveTable(*t_carts, path_carts);
                        db_saveTable(*t_products, path_products);

                        con_printColor("Successfuly added to the table! (Press Any Key) . . .", FG_GREEN);
                        con_anyKey();
                        break;
                    }

                    break;
                case 2:
                    break;
                default:
                    continue;
                }
            }
        }
        else
        {
            // TODO:
            switch (n_input)
            {

            case 1:
                break;
            default:
                continue;
            }
        }

        break;
    }
}

/*
    ========= End Search ==========
*/

/*
    ======== Cart ========
*/
void view_cart()
{
    int n_input;
    int n_totalPrice;
    int n_currentPrice;
    int n_found;

    if (!u_activeUser->is_login)
        return;

    while (true)
    {
        n_found = 0;

        n_totalPrice = 0;

        con_clearScr();

        printf("Cart\n");

        vis_printBars(V_BAR, con_getSize()->x);

        for (int i = 0; i < t_carts->row_count; i++)
        {
            n_currentPrice = 0;

            if (!t_carts->rows[i].is_null)
            {
                if (strcmp(t_carts->rows[i].elements[db_getColIdx(*t_carts, "username")], u_activeUser->username) == 0)
                {
                    (void)printf("\n%s[Cart Item ID:%d]%s", FG_MAGENTA, t_carts->rows[i].id, FG_DEFAULT);
                    (void)printf("\n%s", db_selectRowWhereId(*t_products, atoi(t_carts->rows[i].elements[db_getColIdx(*t_carts, "product_id")]))->elements[db_getColIdx(*t_products, "name")]);
                    (void)printf("\n%sQty:%s%s", FG_GREEN, t_carts->rows[i].elements[db_getColIdx(*t_carts, "quantity")], FG_DEFAULT);
                    n_currentPrice =
                        atoi(t_carts->rows[i].elements[db_getColIdx(*t_carts, "quantity")]) *
                        atoi(db_selectRowWhereId(*t_products, atoi(
                                                                  t_carts->rows[i].elements[db_getColIdx(*t_carts, "product_id")]))
                                 ->elements[db_getColIdx(*t_products, "price")]);
                    (void)printf("\n%sTotal: %sRp%d\n", FG_GREEN, FG_DEFAULT, n_currentPrice);

                    n_totalPrice += n_currentPrice;

                    n_found++;
                }
            }
        }

        if (n_found <= 0)
        {
            con_printColor("You don't have anything on your cart!\n", FG_ERROR);
        }

        vis_printBars(V_BAR, con_getSize()->x);
        con_printColor("Total Price: ", FG_GREEN);
        printf("Rp%d\n", n_totalPrice);

        vis_printBars(V_BAR, con_getSize()->x);
        if (n_found <= 0)
            vis_printListMenu(1, "Exit");
        else
            vis_printListMenu(3, "Edit Cart Item", "Checkout All", "Exit");
        vis_printBars(V_BAR, con_getSize()->x);

        con_printColor("Selection: ", FG_PROMPT);
        n_input = con_inputInt();

        if (n_found <= 0)
        {
            switch (n_input)
            {
            case 1:
                break;
            default:
                continue;
            }
        }
        else
        {

            switch (n_input)
            {
            case 1:
                // edit cart items
                view_cartEditItems();
                continue;
            case 2:
                // checkout
                view_checkout();
                continue;
            case 3:
                break;
            default:
                continue;
            }
        }

        break;
    }
}

void view_cartEditItems()
{
    if (!u_activeUser->is_login)
        return;

    int n_input;
    int n_totalPrice;

    char *quantityInput = (char *)malloc(sizeof(char) * 255);
    int old_quantity, new_quantity;

    db_Row *cart_row = NULL;
    db_Row *product_row = NULL;

    con_printColor("Enter cart ID you want to edit: ", FG_PROMPT);
    n_input = con_inputInt();

    cart_row = db_selectRowWhereId(*t_carts, n_input);
    if (!cart_row)
    {
        con_printColor("Invalid cart ID! (Press Any Key) . . .", FG_ERROR);
        con_anyKey();
        return;
    }

    if (strcmp(cart_row->elements[db_getColIdx(*t_carts, "username")], u_activeUser->username) != 0)
    {
        con_printColor("Invalid cart ID! (Press Any Key) . . .", FG_ERROR);
        con_anyKey();
        return;
    }

    con_clearScr();

    printf("Edit your cart item quantity\n");
    vis_printBars(V_BAR, con_getSize()->x);

    (void)printf("\n%s[Cart Item ID:%d]%s", FG_MAGENTA, cart_row->id, FG_DEFAULT);
    (void)printf("\n%s", db_selectRowWhereId(*t_products, atoi(cart_row->elements[db_getColIdx(*t_carts, "product_id")]))->elements[db_getColIdx(*t_products, "name")]);
    (void)printf("\n%sQty:%s%s", FG_GREEN, cart_row->elements[db_getColIdx(*t_carts, "quantity")], FG_DEFAULT);
    n_totalPrice =
        atoi(cart_row->elements[db_getColIdx(*t_carts, "quantity")]) *
        atoi(db_selectRowWhereId(*t_products, atoi(
                                                  cart_row->elements[db_getColIdx(*t_carts, "product_id")]))
                 ->elements[db_getColIdx(*t_products, "price")]);
    (void)printf("\n%sTotal: %sRp%d\n", FG_GREEN, FG_DEFAULT, n_totalPrice);

    vis_printBars(V_BAR, con_getSize()->x);
    printf("- Make it 0 to delete it"
           "- Enter to skip / discard edit\n");
    vis_printBars(V_BAR, con_getSize()->x);

    con_printColor("Set quantity to: ", FG_PROMPT);
    quantityInput = con_inputStr();

    if (strlen(quantityInput) <= 0)
    {
        con_printColor("Cart item did not edited (Press Any Key) . . .", FG_ERROR);
        con_anyKey();
        return;
    }

    new_quantity = atoi(quantityInput);

    if (new_quantity < 0)
    {
        con_printColor("Invalid quantity! (Press Any Key) . . .", FG_ERROR);
        con_anyKey();
        return;
    }

    old_quantity = atoi(cart_row->elements[db_getColIdx(*t_carts, "quantity")]);

    product_row = db_selectRowWhereId(*t_products, atoi(cart_row->elements[db_getColIdx(*t_carts, "product_id")]));
    product_row->elements[db_getColIdx(*t_products, "stock")] =
        strdup(util_intToStr((atoi(product_row->elements[db_getColIdx(*t_products, "stock")]) + old_quantity) - new_quantity));

    cart_row->elements[db_getColIdx(*t_carts, "quantity")] = util_intToStr(new_quantity);

    db_saveTable(*t_carts, path_carts);
    db_saveTable(*t_products, path_products);

    con_printColor("Cart successfuly edited! (Press Any Key) . . .", FG_GREEN);
    con_anyKey();
}

/*
    ======== End Cart ========
*/

/*
    ========= Orders ========
*/
void view_orders()
{

    if (!u_activeUser->is_login)
        return;

    int n_found_od, n_found_pkg;
    int n_input;

    db_Row *return_row = NULL;
    db_Row *request_row = NULL;
    db_Row *detail_row = NULL;
    db_Row *detailProduct_row = NULL;

    // user view
    if (!u_activeUser->is_seller)
    {
        while (true)
        {
            return_row = NULL;
            request_row = NULL;
            detail_row = NULL;

            con_clearScr();

            n_found_od = 0;

            printf("My orders and status\n");

            vis_printBars(V_BAR, con_getSize()->x);

            for (int i = 0; i < t_orders->row_count; i++)
            {
                n_found_pkg = 0;

                if (!t_orders->rows[i].is_null)
                {
                    if (strcmp(t_orders->rows[i].elements[db_getColIdx(*t_orders, "username")], u_activeUser->username) == 0)
                    {
                        int daysLeft = dt_getDaysBetween(
                            *dt_getTimeNow(),
                            *dt_strToDate(strdup(t_orders->rows[i].elements[db_getColIdx(*t_orders, "delivery_date")])));
                        bool isDelivered = (daysLeft > 0)
                                               ? false
                                               : true;
                        printf("\n%s[Order ID: %d]%s\n", FG_MAGENTA, t_orders->rows[i].id, FG_DEFAULT);

                        con_printColor("Products: ", FG_GREEN);
                        int j;
                        int product_count = db_getHowManyWhere(*t_orderPackages, db_getColIdx(*t_orderPackages, "order_id"), util_intToStr(t_orders->rows[i].id));
                        for (j = 0; j < t_orderPackages->row_count; j++)
                        {
                            if (!t_orderPackages->rows[j].is_null)
                            {
                                if (strcmp(t_orderPackages->rows[j].elements[db_getColIdx(*t_orderPackages, "order_id")], util_intToStr(t_orders->rows[i].id)) == 0)
                                {
                                    printf("%s", db_selectRowWhereId(*t_products, atoi(t_orderPackages->rows[j].elements[db_getColIdx(*t_orderPackages, "product_id")]))->elements[db_getColIdx(*t_products, "name")]);
                                    if (n_found_pkg >= 2)
                                    {
                                        printf(", etc...");
                                        break;
                                    }

                                    if (n_found_pkg < product_count - 1)
                                    {
                                        printf(", ");
                                    }

                                    n_found_pkg++;
                                }
                            }
                        }

                        printf("\n");

                        con_printColor("Status: ", FG_GREEN);
                        request_row = db_selectFirstRowWhere(*t_returnRequest, db_getColIdx(*t_returnRequest, "order_id"), util_intToStr(t_orders->rows[i].id));
                        if (!request_row)
                        {
                            if (isDelivered)
                                con_printColor("Shipped\n", FG_CYAN);
                            else
                                con_printColor("On delivery\n", FG_YELLOW);

                            request_row = NULL;
                        }
                        else
                        {
                            con_printColor("Request return pending\n", FG_ERROR);
                            request_row = NULL;
                        }

                        con_printColor("Ordered on: ", FG_GREEN);
                        printf("%s\n", t_orders->rows[i].elements[db_getColIdx(*t_orders, "order_date")]);

                        if (!isDelivered)
                        {
                            con_printColor("Estimated delivery: ", FG_GREEN);
                            printf("In %d day%s\n", daysLeft, (daysLeft > 1) ? "s" : "");
                        }
                        else
                        {
                            con_printColor("Delivered on: ", FG_GREEN);
                            printf("%s\n", t_orders->rows[i].elements[db_getColIdx(*t_orders, "delivery_date")]);
                        }

                        n_found_od++;
                    }
                }
            }

            if (n_found_od <= 0)
                con_printColor("Currently no orders shipped!\n", FG_ERROR);

            vis_printBars(V_BAR, con_getSize()->x);

            if (n_found_od <= 0)
                vis_printListMenu(1, "Exit");
            else
                vis_printListMenu(3, "View order details", "Request return", "Exit");

            vis_printBars(V_BAR, con_getSize()->x);

            con_printColor("Selection: ", FG_PROMPT);
            n_input = con_inputInt();

            if (n_found_od <= 0)
            {
                switch (n_input)
                {
                case 1:
                    break;
                default:
                    continue;
                }
            }
            else
            {
                switch (n_input)
                {
                case 1:
                    // view details

                    con_printColor("Enter order ID: ", FG_PROMPT);
                    n_input = con_inputInt();

                    detail_row = db_selectRowWhereId(*t_orders, n_input);
                    if (!detail_row)
                    {
                        con_printColor("Order ID not found! (Press Any Key) . . .", FG_ERROR);
                        detail_row = NULL;
                        con_anyKey();
                        continue;
                    }

                    if (strcmp(detail_row->elements[db_getColIdx(*t_orders, "username")], u_activeUser->username) != 0)
                    {
                        con_printColor("Order ID not found! (Press Any Key) . . .", FG_ERROR);
                        detail_row = NULL;
                        con_anyKey();
                        continue;
                    }

                    while (true)
                    {

                        con_clearScr();

                        printf("Order details\n");
                        vis_printBars(V_BAR, con_getSize()->x);
                        printf("\n%s[Order ID: %d]%s\n", FG_MAGENTA, detail_row->id, FG_DEFAULT);
                        vis_printBars(V_BAR, con_getSize()->x);

                        con_printColor("Recipient: ", FG_GREEN);
                        printf("%s\n", detail_row->elements[db_getColIdx(*t_orders, "username")]);
                        con_printColor("Address: ", FG_GREEN);
                        printf("%s\n", detail_row->elements[db_getColIdx(*t_orders, "address")]);
                        con_printColor("Payment: ", FG_GREEN);
                        printf("%s\n", db_selectRowWhereId(*t_bankAccounts, atoi(detail_row->elements[db_getColIdx(*t_orders, "card_id")]))->elements[db_getColIdx(*t_bankAccounts, "bank")]);
                        con_printColor("Agent: ", FG_GREEN);
                        printf("%s\n", db_selectRowWhereId(*t_agents, atoi(detail_row->elements[db_getColIdx(*t_orders, "agent_id")]))->elements[db_getColIdx(*t_agents, "name")]);

                        vis_printBars(V_BAR, con_getSize()->x);
                        printf("Products\n");
                        vis_printBars(V_BAR, con_getSize()->x);

                        for (int i = 0; i < t_orderPackages->row_count; i++)
                        {
                            detailProduct_row = NULL;
                            if (!t_orderPackages->rows[i].is_null)
                            {
                                if (strcmp(t_orderPackages->rows[i].elements[db_getColIdx(*t_orderPackages, "order_id")], util_intToStr(detail_row->id)) == 0)
                                {
                                    detailProduct_row = db_selectRowWhereId(*t_products, atoi(t_orderPackages->rows[i].elements[db_getColIdx(*t_orderPackages, "product_id")]));
                                    printf("\n%s\n", detailProduct_row->elements[db_getColIdx(*t_products, "name")]);
                                    printf("%sRp%s %sx%s%s\n",
                                           FG_GREEN,
                                           detailProduct_row->elements[db_getColIdx(*t_products, "price")],
                                           FG_YELLOW,
                                           t_orderPackages->rows[i].elements[db_getColIdx(*t_orderPackages, "quantity")],
                                           FG_DEFAULT);
                                }
                            }
                        }

                        vis_printBars(V_BAR, con_getSize()->x);
                        vis_printListMenu(1, "Exit");
                        vis_printBars(V_BAR, con_getSize()->x);

                        con_printColor("Selection: ", FG_PROMPT);
                        n_input = con_inputInt();

                        switch (n_input)
                        {
                        case 1:
                            break;
                        default:
                            continue;
                        }

                        break;
                    }

                    continue;
                case 2:
                    // return
                    con_printColor("Enter order ID you want to return: ", FG_PROMPT);
                    n_input = con_inputInt();
                    return_row = db_selectRowWhereId(*t_orders, n_input);
                    if (!return_row)
                    {
                        con_printColor("Order ID not found! (Press Any Key) . . .", FG_ERROR);
                        return_row = NULL;
                        con_anyKey();
                        continue;
                    }

                    if (strcmp(return_row->elements[db_getColIdx(*t_orders, "username")], u_activeUser->username) != 0)
                    {
                        con_printColor("Invalid order ID! (Press Any Key) . . .", FG_ERROR);
                        return_row = NULL;
                        con_anyKey();
                        continue;
                    }

                    if (dt_getDaysBetween(*dt_getTimeNow(), *dt_strToDate(return_row->elements[db_getColIdx(*t_orders, "delivery_date")])) > 0)
                    {
                        con_printColor("Order is still on shipment! (Press Any Key) . . .", FG_ERROR);
                        return_row = NULL;
                        con_anyKey();
                        continue;
                    }

                    if (dt_getDaysBetween(*dt_getTimeNow(), *dt_strToDate(return_row->elements[db_getColIdx(*t_orders, "delivery_date")])) < -7)
                    {
                        con_printColor("Can't return order that's been delivered over 7 days! (Press Any Key) . . .", FG_ERROR);
                        return_row = NULL;
                        con_anyKey();
                        continue;
                    }

                    request_row = db_newRow(t_returnRequest->col_count);
                    request_row->id = db_getHighestId(*t_returnRequest) + 1;
                    request_row->elements[db_getColIdx(*t_returnRequest, "order_id")] = util_intToStr(return_row->id);

                    db_insertRow(t_returnRequest, *request_row);
                    db_saveTable(*t_returnRequest, path_returnRequest);

                    con_printColor("Request return sent to seller!\nPlease wait until the seller approves it\nYour balance will be updated after the seller approves it (Press Any Key) . . .", FG_GREEN);
                    return_row = NULL;
                    request_row = NULL;
                    con_anyKey();

                    continue;
                case 3:
                    break;
                default:
                    continue;
                }
            }

            break;
        }
    }
}

/*
    ========= End Orders ========
*/

/*
    ========= Checkout =========
*/

void view_checkout()
{
    int n_input;

    int n_bankAccountId = 0;
    int n_address_id = 0;
    int n_agent_id = 0;

    db_Row *address_row = NULL;
    db_Row *bankAccount_row = NULL;
    db_Row *agent_row = NULL;
    db_Row *order_row = NULL;

    dt_DateTime *deliveryDate;

    double lat;
    double lon;

    int totalPrice;
    int totalFee;
    int totalWeight;
    int deliveryDays;
    double totalKM;

    if (!u_activeUser->is_login)
        return;

    while (true)
    {
        totalPrice = 0;
        totalFee = 0;
        totalWeight = 0;
        totalKM = 0.0;

        con_clearScr();

        con_printColor("Checkout\n", FG_GREEN);
        vis_printBars(V_BAR, con_getSize()->x);

        for (int i = 0; i < t_carts->row_count; i++)
        {
            if (!t_carts->rows[i].is_null)
            {
                if (strcmp(t_carts->rows[i].elements[db_getColIdx(*t_carts, "username")], u_activeUser->username) == 0)
                {
                    (void)printf("\n%s\n", db_selectRowWhereId(*t_products, atoi(t_carts->rows[i].elements[db_getColIdx(*t_carts, "product_id")]))->elements[db_getColIdx(*t_products, "name")]);
                    (void)printf("%sRp%s ", FG_YELLOW, db_selectRowWhereId(*t_products, atoi(t_carts->rows[i].elements[db_getColIdx(*t_carts, "product_id")]))->elements[db_getColIdx(*t_products, "price")]);
                    (void)printf("%sx%s%s\n", FG_GREEN, t_carts->rows[i].elements[db_getColIdx(*t_carts, "quantity")], FG_DEFAULT);

                    totalPrice +=
                        (atoi(t_carts->rows[i].elements[db_getColIdx(*t_carts, "quantity")]) *
                         atoi(db_selectRowWhereId(*t_products, atoi(t_carts->rows[i].elements[db_getColIdx(*t_carts, "product_id")]))->elements[db_getColIdx(*t_products, "price")]));
                    totalWeight += atoi(db_selectRowWhereId(*t_products, atoi(t_carts->rows[i].elements[db_getColIdx(*t_carts, "product_id")]))->elements[db_getColIdx(*t_products, "weight")]);
                }
            }
        }

        vis_printBars(V_BAR, con_getSize()->x);

        // Shipping Address
        con_printColor("Shipping Address: ", FG_GREEN);
        if (!n_address_id)
            printf("%s- (Not Set)%s\n", FG_RED, FG_DEFAULT);
        else
        {
            address_row = db_selectRowWhereId(*t_address, n_address_id);
            if (!address_row)
            {
                printf("%s- (Error)%s\n", FG_RED, FG_DEFAULT);
                n_address_id = 0;
            }
            else
            {
                printf("%s(%s) %s", FG_YELLOW, address_row->elements[db_getColIdx(*t_address, "name")], FG_DEFAULT);
                printf("%s ", address_row->elements[db_getColIdx(*t_address, "full_address")]);

                sscanf(address_row->elements[db_getColIdx(*t_address, "latitude")], "%lf", &lat);
                sscanf(address_row->elements[db_getColIdx(*t_address, "longitude")], "%lf", &lon);

                totalKM = gps_distanceLL(lat, lon, GPS_STORE_LOC_LAT, GPS_STORE_LOC_LON);
                printf("%s(%.2lfkm)%s\n", FG_GREEN, totalKM, FG_DEFAULT);
            }
        }

        // Bank Account
        con_printColor("Payment: ", FG_GREEN);
        if (!n_bankAccountId)
            printf("%s- (Not Set)%s\n", FG_RED, FG_DEFAULT);
        else
        {
            bankAccount_row = db_selectRowWhereId(*t_bankAccounts, n_bankAccountId);
            if (!bankAccount_row)
            {
                printf("%s- (Error)%s\n", FG_RED, FG_DEFAULT);
                n_bankAccountId = 0;
            }
            else
            {
                printf("%s[%s]%s %s %s(Rp%s)%s\n",
                       FG_BLUE,
                       bankAccount_row->elements[db_getColIdx(*t_bankAccounts, "bank")],
                       FG_DEFAULT,
                       bankAccount_row->elements[db_getColIdx(*t_bankAccounts, "number")],
                       FG_GREEN,
                       bankAccount_row->elements[db_getColIdx(*t_bankAccounts, "balance")],
                       FG_DEFAULT);
            }
        }

        // Shipping Agent
        con_printColor("Agent: ", FG_GREEN);
        if (!n_agent_id)
            printf("%s- (Not Set)%s\n", FG_RED, FG_DEFAULT);
        else
        {
            agent_row = db_selectRowWhereId(*t_agents, n_agent_id);
            if (!agent_row)
            {
                printf("%s- (Error)%s\n", FG_RED, FG_DEFAULT);
                n_agent_id = 0;
            }
            else
            {
                printf("%s ", agent_row->elements[db_getColIdx(*t_agents, "name")]);
                if (!n_address_id)
                    printf("\n");
                else
                {
                    totalFee =
                        (double)totalWeight / (double)1000 * (double)atoi(agent_row->elements[db_getColIdx(*t_agents, "per_kg")]) +
                        (double)totalKM * (double)atoi(agent_row->elements[db_getColIdx(*t_agents, "per_km")]);

                    printf("%s(Fee: Rp%d)%s\n", FG_YELLOW, totalFee, FG_DEFAULT);
                }
            }
        }

        vis_printBars(V_BAR, con_getSize()->x);
        printf("%sTotal price:%s Rp%d\n", FG_GREEN, FG_WHITE, totalPrice);
        con_printColor("Shipping fee: ", FG_GREEN);
        if (n_agent_id)
            printf("Rp%d\n", totalFee);
        else
            con_printColor("-\n", FG_ERROR);
        vis_printBars(V_BAR, con_getSize()->x);
        con_printColor("You need to pay: ", FG_GREEN);
        if (n_agent_id)
            printf("Rp%d\n", totalPrice + totalFee);
        else
            con_printColor("-\n", FG_ERROR);
        vis_printBars(V_BAR, con_getSize()->x);

        if (!n_address_id || !n_bankAccountId || !n_agent_id)
            vis_printListMenu(4, "Edit shipping address", "Edit payment", "Edit shipping agent", "Back");
        else
            vis_printListMenu(5, "Place Order", "Edit shipping address", "Edit payment", "Edit shipping agent", "Back");

        vis_printBars(V_BAR, con_getSize()->x);

        con_printColor("Selection: ", FG_PROMPT);
        n_input = con_inputInt();

        if (!n_address_id || !n_bankAccountId || !n_agent_id)
        {
            switch (n_input)
            {
            case 1:
                view_checkoutEditAddress(&n_address_id);
                continue;
            case 2:
                view_checkoutEditPayment(&n_bankAccountId);
                continue;
            case 3:
                view_checkoutEditAgent(&n_agent_id);
                continue;
            case 4:
                break;
            default:
                continue;
            }
        }
        else
        {
            switch (n_input)
            {
            case 1:
                // place order

                // Calculate delivery date
                deliveryDays = rd_getRangeInt(1, 2);
                deliveryDays += ((int)totalKM / rd_getRangeInt(30, 45));
                deliveryDays += dt_dateTimeToDays(*dt_getTimeNow());

                deliveryDate = dt_daysToDateTime(deliveryDays);
                deliveryDate->hour = rd_getRangeInt(7, 20);
                deliveryDate->minute = rd_getRangeInt(0, 59);

                // Check if bank balance sufficient
                if (atoi(bankAccount_row->elements[db_getColIdx(*t_bankAccounts, "balance")]) < totalPrice + totalFee)
                {
                    con_printColor("Insufficient bank balance! Please use another payment method (Press Any Key) . . .", FG_ERROR);
                    con_anyKey();
                    continue;
                }

                // substract the payment
                bankAccount_row->elements[db_getColIdx(*t_bankAccounts, "balance")] =
                    util_intToStr(atoi(bankAccount_row->elements[db_getColIdx(*t_bankAccounts, "balance")]) - (totalPrice + totalFee));

                // new order
                order_row = db_newRow(t_orders->col_count);
                order_row->id = db_getHighestId(*t_orders) + 1;
                order_row->elements[db_getColIdx(*t_orders, "username")] = strdup(u_activeUser->username);
                order_row->elements[db_getColIdx(*t_orders, "address")] = strdup(address_row->elements[db_getColIdx(*t_address, "full_address")]);
                order_row->elements[db_getColIdx(*t_orders, "latitude")] = strdup(address_row->elements[db_getColIdx(*t_address, "latitude")]);
                order_row->elements[db_getColIdx(*t_orders, "longitude")] = strdup(address_row->elements[db_getColIdx(*t_address, "longitude")]);
                order_row->elements[db_getColIdx(*t_orders, "card_id")] = util_intToStr(bankAccount_row->id);
                order_row->elements[db_getColIdx(*t_orders, "agent_id")] = util_intToStr(agent_row->id);
                order_row->elements[db_getColIdx(*t_orders, "order_date")] = dt_dateTimeToStr(*dt_getTimeNow());
                order_row->elements[db_getColIdx(*t_orders, "delivery_date")] = dt_dateTimeToStr(*deliveryDate);
                order_row->elements[db_getColIdx(*t_orders, "total_price")] = util_intToStr(totalPrice);
                order_row->elements[db_getColIdx(*t_orders, "shipping_fee")] = util_intToStr(totalFee);

                // process the packages
                for (int i = 0; i < t_carts->row_count; i++)
                {
                    if (!t_carts->rows[i].is_null)
                    {
                        if (strcmp(t_carts->rows[i].elements[db_getColIdx(*t_carts, "username")], u_activeUser->username) == 0)
                        {
                            // add the products in the cart to package
                            db_Row *package_row = db_newRow(t_orderPackages->col_count);
                            package_row->id = db_getHighestId(*t_orderPackages) + 1;
                            package_row->elements[db_getColIdx(*t_orderPackages, "order_id")] = util_intToStr(order_row->id);
                            package_row->elements[db_getColIdx(*t_orderPackages, "product_id")] = t_carts->rows[i].elements[db_getColIdx(*t_carts, "product_id")];
                            package_row->elements[db_getColIdx(*t_orderPackages, "quantity")] = t_carts->rows[i].elements[db_getColIdx(*t_carts, "quantity")];
                            db_insertRow(t_orderPackages, *package_row);

                            // add the numbers sold
                            db_Row *product_row = db_selectRowWhereId(*t_products, atoi(t_carts->rows[i].elements[db_getColIdx(*t_carts, "product_id")]));
                            product_row->elements[db_getColIdx(*t_products, "numbers_sold")] = util_intToStr(
                                atoi(product_row->elements[db_getColIdx(*t_products, "numbers_sold")]) + atoi(t_carts->rows[i].elements[db_getColIdx(*t_carts, "quantity")]));

                            // clear the cart
                            t_carts->rows[i].is_null = true;
                        }
                    }
                }

                db_insertRow(t_orders, *order_row);

                db_saveTable(*t_bankAccounts, path_bankAccounts);
                db_saveTable(*t_orders, path_orders);
                db_saveTable(*t_orderPackages, path_orderPackages);
                db_saveTable(*t_products, path_products);
                db_saveTable(*t_carts, path_carts);

                printf("%sOrder successfuly placed with %sID: %d\nEstimated delivery in %d day(s)\n%sThanks for shopping!!! (Press Any Key) . . .", FG_GREEN, FG_YELLOW, order_row->id,
                       dt_getDaysBetween(
                           *dt_getTimeNow(),
                           *dt_strToDate(strdup(order_row->elements[db_getColIdx(*t_orders, "delivery_date")]))),
                       FG_DEFAULT);
                con_anyKey();

                break;
            case 2:
                view_checkoutEditAddress(&n_address_id);
                continue;
            case 3:
                view_checkoutEditPayment(&n_bankAccountId);
                continue;
            case 4:
                view_checkoutEditAgent(&n_agent_id);
                continue;
            case 5:
                break;
            default:
                continue;
            }
        }

        break;
    }
}

void view_checkoutEditAddress(int *p_address)
{

    if (!u_activeUser->is_login)
        return;

    int n_input;
    int n_found;

    db_Row *address_row = NULL;

    while (true)
    {
        con_clearScr();

        n_found = 0;

        (void)printf("Select your shipping address\n");
        vis_printBars(V_BAR, con_getSize()->x);

        for (int i = 0; i < t_address->row_count; i++)
        {
            if (!t_address->rows[i].is_null)
            {
                if (strcmp(t_address->rows[i].elements[db_getColIdx(*t_address, "username")], u_activeUser->username) == 0)
                {
                    n_found++;
                    printf("\n%s[Address ID: %d]%s", FG_MAGENTA, t_address->rows[i].id, FG_DEFAULT);
                    printf("\n%s%s%s", FG_CYAN, t_address->rows[i].elements[db_getColIdx(*t_address, "name")], FG_DEFAULT);
                    printf("\n%s\n", t_address->rows[i].elements[db_getColIdx(*t_address, "full_address")]);
                }
            }
        }

        if (n_found <= 0)
        {
            (void)printf("\n%sYou don't have any shipping address, please add it in settings!\n%s", FG_ERROR, FG_DEFAULT);
        }

        vis_printBars(V_BAR, con_getSize()->x);

        if (n_found <= 0)
            vis_printListMenu(1, "Back");
        else
            vis_printListMenu(2, "Select address with ID", "Back");

        vis_printBars(V_BAR, con_getSize()->x);

        con_printColor("Selection: ", FG_PROMPT);
        n_input = con_inputInt();

        if (n_found <= 0)
        {
            switch (n_input)
            {
            case 1:
                break;
            default:
                continue;
            }
        }
        else
        {
            switch (n_input)
            {
            case 1:
                con_printColor("Select Address ID: ", FG_PROMPT);
                n_input = con_inputInt();
                address_row = db_selectRowWhereId(*t_address, n_input);

                if (!address_row)
                {
                    con_printColor("Invalid address ID! (Press Any Key) . . .", FG_ERROR);
                    con_anyKey();
                    continue;
                }

                if (strcmp(
                        u_activeUser->username,
                        address_row->elements[db_getColIdx(*t_address, "username")]) == 0)
                {
                    con_printColor("Address successfuly set! (Press Any Key) . . .", FG_GREEN);
                    *p_address = n_input;
                    con_anyKey();
                    break;
                }
                else
                {
                    con_printColor("Invalid address ID! (Press Any Key) . . .", FG_ERROR);
                    con_anyKey();
                    continue;
                }
                break;
            case 2:
                break;
            default:
                continue;
            }
        }

        break;
    }
}

void view_checkoutEditPayment(int *p_bankAccount)
{
    if (!u_activeUser->is_login)
        return;

    int n_input;
    int n_found;

    db_Row *payment_row = NULL;
    db_Row *bankAccountRow = NULL;

    while (true)
    {
        con_clearScr();

        n_found = 0;

        (void)printf("Select your payment type\n");
        vis_printBars(V_BAR, con_getSize()->x);

        for (int i = 0; i < t_userPayments->row_count; i++)
        {
            if (!t_userPayments->rows[i].is_null)
            {
                if (strcmp(t_userPayments->rows[i].elements[db_getColIdx(*t_userPayments, "username")], u_activeUser->username) == 0)
                {

                    db_Row *account_row = db_selectRowWhereId(*t_bankAccounts, atoi(t_userPayments->rows[i].elements[db_getColIdx(*t_userPayments, "account_id")]));
                    if (!account_row)
                        continue;

                    printf("\n%s[Payment ID: %d]%s", FG_MAGENTA, t_userPayments->rows[i].id, FG_DEFAULT);
                    printf("\n%s%s%s", FG_CYAN, account_row->elements[db_getColIdx(*t_bankAccounts, "bank")], FG_DEFAULT);
                    printf("\n%s", account_row->elements[db_getColIdx(*t_bankAccounts, "number")]);
                    printf("\n%s%s%s\n", FG_YELLOW, account_row->elements[db_getColIdx(*t_bankAccounts, "owner")], FG_DEFAULT);

                    n_found++;
                }
            }
        }

        if (n_found <= 0)
        {
            (void)printf("\n%sYou don't have any payment connected to your account!, please add it in settings!\n%s", FG_ERROR, FG_DEFAULT);
        }

        vis_printBars(V_BAR, con_getSize()->x);

        if (n_found <= 0)
            vis_printListMenu(1, "Back");
        else
            vis_printListMenu(2, "Select paymnet with ID", "Back");

        vis_printBars(V_BAR, con_getSize()->x);

        con_printColor("Selection: ", FG_PROMPT);
        n_input = con_inputInt();

        if (n_found <= 0)
        {
            switch (n_input)
            {
            case 1:
                break;
            default:
                continue;
            }
        }
        else
        {
            switch (n_input)
            {
            case 1:
                con_printColor("Select Payment ID: ", FG_PROMPT);
                n_input = con_inputInt();
                payment_row = db_selectRowWhereId(*t_userPayments, n_input);

                if (!payment_row)
                {
                    con_printColor("Invalid payment ID! (Press Any Key) . . .", FG_ERROR);
                    con_anyKey();
                    continue;
                }

                if (strcmp(
                        u_activeUser->username,
                        payment_row->elements[db_getColIdx(*t_userPayments, "username")]) == 0)
                {
                    bankAccountRow = db_selectRowWhereId(*t_bankAccounts, atoi(payment_row->elements[db_getColIdx(*t_userPayments, "account_id")]));
                    if (!bankAccountRow)
                    {
                        con_printColor("Error connecting to the bank! (Press Any Key) . . .", FG_ERROR);
                        con_anyKey();
                        continue;
                    }

                    con_printColor("Address successfuly set! (Press Any Key) . . .", FG_GREEN);
                    *p_bankAccount = atoi(payment_row->elements[db_getColIdx(*t_userPayments, "account_id")]);
                    con_anyKey();
                    break;
                }
                else
                {
                    con_printColor("Invalid payment ID! (Press Any Key) . . .", FG_ERROR);
                    con_anyKey();
                    continue;
                }
                break;
            case 2:
                break;
            default:
                continue;
            }
        }

        break;
    }
}

void view_checkoutEditAgent(int *p_agent)
{
    if (!u_activeUser->is_login)
        return;

    int n_input;
    db_Row *agent_row;

    while (true)
    {
        con_clearScr();

        printf("Select shipping agent\n");
        vis_printBars(V_BAR, con_getSize()->x);

        for (int i = 0; i < t_agents->row_count; i++)
        {
            if (!t_agents->rows[i].is_null)
            {
                printf("\n%s[Agent ID: %d]%s", FG_MAGENTA, t_agents->rows[i].id, FG_DEFAULT);
                printf("\n%s%s%s", FG_CYAN, t_agents->rows[i].elements[db_getColIdx(*t_agents, "name")], FG_DEFAULT);
                printf("\n%s", t_agents->rows[i].elements[db_getColIdx(*t_agents, "description")]);
                printf("\n%sRp%s/kg%s", FG_YELLOW, t_agents->rows[i].elements[db_getColIdx(*t_agents, "per_kg")], FG_DEFAULT);
                printf("\n%sRp%s/km%s\n", FG_YELLOW, t_agents->rows[i].elements[db_getColIdx(*t_agents, "per_km")], FG_DEFAULT);
            }
        }

        vis_printBars(V_BAR, con_getSize()->x);
        vis_printListMenu(2, "Select agent with ID", "Exit");
        vis_printBars(V_BAR, con_getSize()->x);

        con_printColor("Selection: ", FG_PROMPT);
        n_input = con_inputInt();

        switch (n_input)
        {
        case 1:
            con_printColor("Select agent ID: ", FG_PROMPT);
            n_input = con_inputInt();

            agent_row = db_selectRowWhereId(*t_agents, n_input);
            if (!agent_row)
            {
                con_printColor("Invalid agent ID! (Press Any Key) . . .", FG_ERROR);
                con_anyKey();
                continue;
            }

            *p_agent = n_input;
            con_printColor("Agent successfuly set! (Press Any Key) . . .", FG_GREEN);

            break;
        case 2:
            break;
        default:
            continue;
        }

        break;
    }
}

/*
    ========= End Checkout =========
*/

/*
    ========= Settings =========
*/

void view_settings()
{
    // ONLY LOGGED IN USER
    if (!u_activeUser->is_login && !u_activeUser->is_seller)
        return;

    int n_input;
    int n_found;

    char *s_pwInput = (char *)malloc(sizeof(char) * 255);  // old
    char *s_npwInput = (char *)malloc(sizeof(char) * 255); // new

    db_Row *bank_row = NULL;

    while (true)
    {
        con_clearScr();

        printf("Account Settings\n");

        vis_printBars(V_BAR, con_getSize()->x);
        vis_printListMenu(4, "Change password", "Edit Addresses", "Bank Accounts", "Exit");
        vis_printBars(V_BAR, con_getSize()->x);

        con_printColor("Selection: ", FG_PROMPT);
        n_input = con_inputInt();

        switch (n_input)
        {
        case 1:
            // Change Password
            con_clearScr();

            printf("Change %s%s's%s password\n", FG_GREEN, u_activeUser->username, FG_DEFAULT);
            vis_printBars(V_BAR, con_getSize()->x);

            con_printColor("Enter current password: ", FG_PROMPT);
            s_pwInput = con_inputStr();

            db_Row *settings_row = db_selectFirstRowWhere(*t_users,
                                                          db_getColIdx(*t_users, "username"),
                                                          u_activeUser->username);

            if (strcmp(settings_row->elements[db_getColIdx(*t_users, "password")], s_pwInput) != 0)
            {
                con_printColor("Wrong current password! (Press Any Key) . . .", FG_ERROR);
                con_anyKey();
                continue;
            }

            con_printColor("Enter new password: ", FG_PROMPT);
            s_npwInput = con_inputStr();

            settings_row->elements[db_getColIdx(*t_users, "password")] = strdup(s_npwInput);

            db_saveTable(*t_users, path_users);

            con_printColor("Password successfuly changed! (Press Any Key) . . .", FG_GREEN);
            con_anyKey();
            continue;
        case 2:
            // Edit Addresses
            while (true)
            {
                n_found = 0;

                con_clearScr();

                printf("%s%s's%s addresses\n", FG_GREEN, u_activeUser->username, FG_DEFAULT);
                vis_printBars(V_BAR, con_getSize()->x);
                for (int i = 0; i < t_address->row_count; i++)
                {
                    if (!t_address->rows[i].is_null)
                    {
                        if (strcmp(t_address->rows[i].elements[db_getColIdx(*t_address, "username")], u_activeUser->username) == 0)
                        {
                            printf("\n%s[Address ID: %d]%s", FG_MAGENTA, t_address->rows[i].id, FG_DEFAULT);
                            printf("\n%s%s%s", FG_CYAN, t_address->rows[i].elements[db_getColIdx(*t_address, "name")], FG_DEFAULT);
                            printf("\n%s", t_address->rows[i].elements[db_getColIdx(*t_address, "full_address")]);
                            printf("\n%s%s, %s%s\n", FG_YELLOW, t_address->rows[i].elements[db_getColIdx(*t_address, "latitude")], t_address->rows[i].elements[db_getColIdx(*t_address, "longitude")], FG_DEFAULT);

                            n_found++;
                        }
                    }
                }

                if (n_found <= 0)
                    con_printColor("You don't have any addresses set up!\n", FG_ERROR);

                vis_printBars(V_BAR, con_getSize()->x);

                if (n_found <= 0)
                    vis_printListMenu(2, "Add Address", "Exit");
                else
                    vis_printListMenu(4, "Add Address", "Edit Address", "Delete Address", "Exit");

                vis_printBars(V_BAR, con_getSize()->x);

                con_printColor("Selection: ", FG_PROMPT);
                n_input = con_inputInt();

                if (n_found <= 0)
                {
                    switch (n_input)
                    {
                    case 1:
                        view_settingsAddAddress();
                        continue;
                    case 2:
                        break;
                    default:
                        continue;
                    }
                }
                else
                {
                    switch (n_input)
                    {
                    case 1:
                        view_settingsAddAddress();
                        continue;
                    case 2:
                        view_settingsEditAddress();
                        continue;
                    case 3:
                        view_settingsDeleteAddress();
                        continue;
                    case 4:
                        break;
                    default:
                        continue;
                    }
                }

                break;
            }
            continue;
        case 3:
            // Edit Bank Accounts
            while (true)
            {
                bank_row = NULL;
                n_found = 0;

                con_clearScr();

                printf("%s%s's%s connected bank accounts\n", FG_GREEN, u_activeUser->username, FG_DEFAULT);
                vis_printBars(V_BAR, con_getSize()->x);
                for (int i = 0; i < t_userPayments->row_count; i++)
                {
                    if (!t_userPayments->rows[i].is_null)
                    {
                        if (strcmp(t_userPayments->rows[i].elements[db_getColIdx(*t_userPayments, "username")], u_activeUser->username) == 0)
                        {
                            bank_row = db_selectRowWhereId(*t_bankAccounts, atoi(t_userPayments->rows[i].elements[db_getColIdx(*t_userPayments, "account_id")]));

                            printf("\n%s[Account ID: %d]%s", FG_MAGENTA, t_userPayments->rows[i].id, FG_DEFAULT);
                            printf("\n%s%s%s", FG_BLUE, bank_row->elements[db_getColIdx(*t_bankAccounts, "bank")], FG_DEFAULT);
                            printf("\n%s", bank_row->elements[db_getColIdx(*t_bankAccounts, "owner")]);
                            printf("\n%s", bank_row->elements[db_getColIdx(*t_bankAccounts, "number")]);
                            printf("\n%sRp%s%s\n", FG_GREEN, bank_row->elements[db_getColIdx(*t_bankAccounts, "balance")], FG_DEFAULT);

                            bank_row = NULL;
                            n_found++;
                        }
                    }
                }

                if (n_found <= 0)
                    con_printColor("You don't have any bank accounts connected!\n", FG_ERROR);

                vis_printBars(V_BAR, con_getSize()->x);

                if (n_found <= 0)
                    vis_printListMenu(2, "Connect Bank Account", "Exit");
                else
                    vis_printListMenu(3, "Connect Bank Account", "Disconnect Bank Account", "Exit");

                vis_printBars(V_BAR, con_getSize()->x);

                con_printColor("Selection: ", FG_PROMPT);
                n_input = con_inputInt();

                if (n_found <= 0)
                {
                    switch (n_input)
                    {
                    case 1:
                        view_settingsConnectBank();
                        continue;
                    case 2:
                        break;
                    default:
                        continue;
                    }
                }
                else
                {
                    switch (n_input)
                    {
                    case 1:
                        view_settingsConnectBank();
                        continue;
                    case 2:
                        view_settingsDisconnectBank();
                        continue;
                    case 3:
                        break;
                    default:
                        continue;
                    }
                }

                break;
            }

            continue;
        case 4:
            break;
        default:
            continue;
        }

        break;
    }
}

// Addresses

void view_settingsAddAddress()
{
    if (!u_activeUser->is_login)
        return;

    char *s_nameInput = (char *)malloc(sizeof(char) * 255);
    char *s_addrInput = (char *)malloc(sizeof(char) * 255);
    char *s_latInput = (char *)malloc(sizeof(char) * 255);
    char *s_lonInput = (char *)malloc(sizeof(char) * 255);

    double d_lat, d_lon;

    db_Row *address_row = db_newRow(t_address->col_count);

    con_clearScr();

    (void)printf("Add new address\n");
    vis_printBars(V_BAR, con_getSize()->x);

    con_printColor("Enter address name: ", FG_PROMPT);
    s_nameInput = con_inputStr();

    con_printColor("Enter full address: ", FG_PROMPT);
    s_addrInput = con_inputStr();

    con_printColor("Enter latitude: ", FG_PROMPT);
    s_latInput = con_inputStr();
    if (!gps_isValidDouble(s_latInput))
    {
        con_printColor("Invalid format", FG_ERROR);
        con_anyKey();
        return;
    }

    con_printColor("Enter longitude: ", FG_PROMPT);
    s_lonInput = con_inputStr();
    if (!gps_isValidDouble(s_lonInput))
    {
        con_printColor("Invalid format", FG_ERROR);
        con_anyKey();
        return;
    }

    address_row->id = db_getHighestId(*t_address) + 1;
    address_row->elements[db_getColIdx(*t_address, "username")] = strdup(u_activeUser->username);
    address_row->elements[db_getColIdx(*t_address, "name")] = strdup(s_nameInput);
    address_row->elements[db_getColIdx(*t_address, "full_address")] = strdup(s_addrInput);
    address_row->elements[db_getColIdx(*t_address, "latitude")] = strdup(s_latInput);
    address_row->elements[db_getColIdx(*t_address, "longitude")] = strdup(s_lonInput);

    db_insertRow(t_address, *address_row);
    db_saveTable(*t_address, path_address);

    con_printColor("Address successfuly added! (Press Any Key) . . .", FG_GREEN);
    con_anyKey();
}

void view_settingsEditAddress()
{
    if (!u_activeUser->is_login)
        return;

    int n_input;
    db_Row *address_row = NULL;

    char *s_name = (char *)malloc(sizeof(char) * 255);
    char *s_address = (char *)malloc(sizeof(char) * 255);
    char *s_lat = (char *)malloc(sizeof(char) * 255);
    char *s_lon = (char *)malloc(sizeof(char) * 255);

    con_printColor("Enter address ID you want to edit: ", FG_PROMPT);
    n_input = con_inputInt();

    address_row = db_selectRowWhereId(*t_address, n_input);
    if (!address_row)
    {
        con_printColor("Invalid address ID! (Press Any Key) . . .", FG_ERROR);
        con_anyKey();
        return;
    }

    if (strcmp(
            u_activeUser->username,
            address_row->elements[db_getColIdx(*t_address, "username")]) != 0)
    {
        con_printColor("Invalid address ID! (Press Any Key) . . .", FG_ERROR);
        con_anyKey();
        return;
    }

    con_clearScr();
    printf("Edit %s Address\n", address_row->elements[db_getColIdx(*t_address, "name")]);
    vis_printBars(V_BAR, con_getSize()->x);

    // name
    con_printColor("Name: ", FG_YELLOW);
    printf("%s\n", address_row->elements[db_getColIdx(*t_address, "name")]);
    con_printColor("Enter new name (enter to skip) :", FG_PROMPT);
    s_name = con_inputStr();
    if (strlen(s_name) > 0)
        address_row->elements[db_getColIdx(*t_address, "name")] = strdup(s_name);

    // full address
    con_printColor("Full Address: ", FG_YELLOW);
    printf("%s\n", address_row->elements[db_getColIdx(*t_address, "full_address")]);
    con_printColor("Enter new full address (enter to skip) :", FG_PROMPT);
    s_address = con_inputStr();
    if (strlen(s_address) > 0)
        address_row->elements[db_getColIdx(*t_address, "full_address")] = strdup(s_address);

    // latitude
    con_printColor("Latitude: ", FG_YELLOW);
    printf("%s\n", address_row->elements[db_getColIdx(*t_address, "latitude")]);
    con_printColor("Enter new latitude (enter to skip) :", FG_PROMPT);
    s_lat = con_inputStr();
    if (strlen(s_lat) > 0)
        address_row->elements[db_getColIdx(*t_address, "latitude")] = strdup(s_lat);

    // longitude
    con_printColor("Longitude: ", FG_YELLOW);
    printf("%s\n", address_row->elements[db_getColIdx(*t_address, "longitude")]);
    con_printColor("Enter new longitude (enter to skip) :", FG_PROMPT);
    s_lon = con_inputStr();
    if (strlen(s_lon) > 0)
        address_row->elements[db_getColIdx(*t_address, "longitude")] = strdup(s_lon);

    db_saveTable(*t_address, path_address);
    con_printColor("Address successfuly updated! (Press Any Key) . . .", FG_GREEN);
    con_anyKey();
}

void view_settingsDeleteAddress()
{
    if (!u_activeUser->is_login)
        return;

    int n_input;
    db_Row *address_row = NULL;

    con_printColor("Enter address ID you want to delete: ", FG_PROMPT);
    n_input = con_inputInt();

    address_row = db_selectRowWhereId(*t_address, n_input);
    if (!address_row)
    {
        con_printColor("Invalid address ID! (Press Any Key) . . .", FG_ERROR);
        con_anyKey();
        return;
    }

    if (strcmp(
            u_activeUser->username,
            address_row->elements[db_getColIdx(*t_address, "username")]) != 0)
    {
        con_printColor("Invalid address ID! (Press Any Key) . . .", FG_ERROR);
        con_anyKey();
        return;
    }

    address_row->is_null = true;
    db_saveTable(*t_address, path_address);

    con_printColor("Address successfuly deleted! (Press Any Key) . . .", FG_GREEN);

    con_anyKey();
}

// Banks

void view_settingsConnectBank()
{
    if (!u_activeUser->is_login)
        return;

    char *s_number = (char *)malloc(sizeof(char) * 255);
    char *s_pin = (char *)malloc(sizeof(char) * 255);

    db_Row *account_row = NULL;

    con_clearScr();

    printf("Connect to Bank Account\n");
    vis_printBars(V_BAR, con_getSize()->x);

    con_printColor("Enter Bank/Account number: ", FG_PROMPT);
    s_number = con_inputStr();

    account_row = db_selectFirstRowWhere(*t_bankAccounts, db_getColIdx(*t_bankAccounts, "number"), s_number);
    if (!account_row)
    {
        con_printColor("We can't found bank/account with that number (Press Any Key) . . .", FG_ERROR);
        con_anyKey();
        return;
    }

    // show account details
    vis_printBars(V_BAR, con_getSize()->x);
    con_printColor("Account found\n", FG_GREEN);
    vis_printBars(V_BAR, con_getSize()->x);

    printf("\n%s%s%s", FG_BLUE, account_row->elements[db_getColIdx(*t_bankAccounts, "bank")], FG_DEFAULT);
    printf("\n%s", account_row->elements[db_getColIdx(*t_bankAccounts, "owner")]);
    printf("\n%s", account_row->elements[db_getColIdx(*t_bankAccounts, "number")]);
    printf("\n%sRp%s%s\n", FG_GREEN, account_row->elements[db_getColIdx(*t_bankAccounts, "balance")], FG_DEFAULT);
    vis_printBars(V_BAR, con_getSize()->x);

    con_printColor("Enter PIN: ", FG_PROMPT);
    s_pin = con_inputPwd();

    if (strcmp(s_pin, account_row->elements[db_getColIdx(*t_bankAccounts, "pin")]) != 0)
    {
        con_printColor("Wrong PIN! Can't connect to this account (Press Any Key) . . .", FG_ERROR);
        con_anyKey();
        return;
    }

    // Connect account
    db_Row *payment_row = db_newRow(t_userPayments->col_count);
    payment_row->id = db_getHighestId(*t_userPayments) + 1;
    payment_row->elements[db_getColIdx(*t_userPayments, "username")] = strdup(u_activeUser->username);
    payment_row->elements[db_getColIdx(*t_userPayments, "account_id")] = util_intToStr(account_row->id);

    db_insertRow(t_userPayments, *payment_row);
    db_saveTable(*t_userPayments, path_userPayments);

    con_printColor("Account successfuly connected! (Press Any Key) . . .", FG_GREEN);
    con_anyKey();
}

void view_settingsDisconnectBank()
{
    if (!u_activeUser->is_login)
        return;

    int n_input;
    char *s_pin = (char *)malloc(sizeof(char) * 255);
    db_Row *payment_row = NULL;
    db_Row *account_row = NULL;

    con_printColor("Enter account ID you want to disconnect: ", FG_PROMPT);
    n_input = con_inputInt();

    payment_row = db_selectRowWhereId(*t_userPayments, n_input);
    if (!payment_row)
    {
        con_printColor("Invalid ID! (Press Any Key) . . .", FG_RED);
        con_anyKey();
        return;
    }

    if (strcmp(payment_row->elements[db_getColIdx(*t_userPayments, "username")], u_activeUser->username) != 0)
    {
        con_printColor("Invalid ID! (Press Any Key) . . .", FG_RED);
        con_anyKey();
        return;
    }

    account_row = db_selectRowWhereId(*t_bankAccounts, atoi(payment_row->elements[db_getColIdx(*t_userPayments, "account_id")]));

    con_clearScr();

    con_printColor("Account you're about to disconnect\n", FG_GREEN);
    vis_printBars(V_BAR, con_getSize()->x);

    printf("\n%s%s%s", FG_BLUE, account_row->elements[db_getColIdx(*t_bankAccounts, "bank")], FG_DEFAULT);
    printf("\n%s", account_row->elements[db_getColIdx(*t_bankAccounts, "owner")]);
    printf("\n%s", account_row->elements[db_getColIdx(*t_bankAccounts, "number")]);
    printf("\n%sRp%s%s\n", FG_GREEN, account_row->elements[db_getColIdx(*t_bankAccounts, "balance")], FG_DEFAULT);
    vis_printBars(V_BAR, con_getSize()->x);

    con_printColor("Enter PIN: ", FG_PROMPT);
    s_pin = con_inputPwd();

    if (strcmp(s_pin, account_row->elements[db_getColIdx(*t_bankAccounts, "pin")]) != 0)
    {
        con_printColor("Wrong PIN! Can't disconnect this account (Press Any Key) . . .", FG_ERROR);
        con_anyKey();
        return;
    }

    payment_row->is_null = true;
    db_saveTable(*t_userPayments, path_userPayments);

    con_printColor("Account successfuly disconnected! (Press Any Key) . . .", FG_GREEN);
    con_anyKey();
}

/*
    ========= End Settings =========
*/

/*
    ========= Admin Manage ==========
*/

// TODO:
void view_adminManageProducts()
{
    if (!u_activeUser->is_login && !u_activeUser->is_seller)
        return;

    while (true)
    {
        con_clearScr();
        db_printTable(*t_products);

        con_anyKey();

        break;
    }
}

void view_adminManageOrders()
{
    if (!u_activeUser->is_login && !u_activeUser->is_seller)
        return;
}

/*
    ========= End Admin Manage =======
*/