// Copyright (c) 2014-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_JSON_PEGTL_INTERNAL_RANGE_HPP
#define TAO_JSON_PEGTL_INTERNAL_RANGE_HPP

#include "../config.hpp"

#include "result_on_found.hpp"
#include "skip_control.hpp"

#include "../analysis/generic.hpp"

namespace TAO_JSON_PEGTL_NAMESPACE::internal
{
   template< result_on_found R, typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
   struct range
   {
      static_assert( Lo <= Hi, "invalid range detected" );

      using analyze_t = analysis::generic< analysis::rule_type::any >;

      template< int Eol >
      static constexpr bool can_match_eol = ( ( ( Lo <= Eol ) && ( Eol <= Hi ) ) == bool( R ) );

      template< typename Input >
      [[nodiscard]] static bool match( Input& in )
      {
         if( !in.empty() ) {
            if( const auto t = Peek::peek( in ) ) {
               if( ( ( Lo <= t.data ) && ( t.data <= Hi ) ) == bool( R ) ) {
                  if constexpr( can_match_eol< Input::eol_t::ch > ) {
                     in.bump( t.size );
                  }
                  else {
                     in.bump_in_this_line( t.size );
                  }
                  return true;
               }
            }
         }
         return false;
      }

      struct inverted
         : range< !R, Peek, Lo, Hi >
      {};
   };

   template< result_on_found R, typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
   inline constexpr bool skip_control< range< R, Peek, Lo, Hi > > = true;

}  // namespace TAO_JSON_PEGTL_NAMESPACE::internal

#endif
