#pragma once
#include <string>
#include <pqxx/pqxx>

#include "../../Public/dbcon.hpp"

struct Item
{
    Varchar name;

    static bool Insert( const Item& item, pqxx::work& tx )
    {
        std::string_view   strView( item.name.data() );
        const pqxx::result result    = tx.exec( "INSERT INTO items VALUES (DEFAULT, $1);", pqxx::params{ strView } );
        const size_t row_count = result.affected_rows();
        tx.commit();
        return row_count;
    }

    static size_t GetIdentifier( const std::string_view name, pqxx::work&& tx )
    {
        pqxx::params params;
        params.append( name );
        return tx.query_value<size_t>( "SELECT id FROM items WHERE name=$1;", params);
    }
};

static TableRegistration<Item> ItemTableRegistration( "Item" );