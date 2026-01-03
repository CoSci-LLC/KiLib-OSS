
#include <KiLib/Rasters/DenseRaster.hpp>



#define OPERATOR(OPERAND, OPERAND2)   \
   DenseRaster<double> operator OPERAND( const DenseRaster<double>& a, const DenseRaster<double>& b ) \
   {  \
      if ( !a.can_perform_operation( b ) ) \
      { \
         throw std::invalid_argument( "Cannot Multiple disimilar rasters" ); \
      } \
      KiLib::Rasters::DenseRaster<double> out( std::make_tuple( a.get_rows(), a.get_cols(), a.get_zindex() )); \
      out.copy_metadata_from(a);\
      for ( size_t i = 0; i < a.get_ndata(); i++ )\
      {\
         out.nodata_mask[i] = a.nodata_mask[i] || b.nodata_mask[i];\
      }\
      std::transform(    \
         EXEC_POLICY, a.data.begin(), a.data.end(), b.data.begin(), out.data.begin(), \
         []( const double& a, const double& b ) \
         { \
               return a OPERAND b; \
         } ); \
      out.set_name( a.get_name() + " " + #OPERAND + " " +b.get_name() );\
      return out; \
   } \
   DenseRaster<double> operator OPERAND( DenseRaster<double>&& a, DenseRaster<double>&& b ) \
   { \
      if ( !a.can_perform_operation( b ) ) \
      {\
         throw std::invalid_argument( "Cannot Multiple disimilar rasters" ); \
      }\
      std::transform(\
         EXEC_POLICY, a.data.begin(), a.data.end(), b.data.begin(), a.data.begin(),\
         []( const double& a, const double& b )\
         {\
               return a OPERAND b;\
         } );\
      for ( size_t i = 0; i < a.get_ndata(); i++ )\
      {\
         a.nodata_mask[i] = a.nodata_mask[i] || b.nodata_mask[i];\
      }\
      a.set_name( a.get_name() + " " + #OPERAND + " " +b.get_name() );\
      return std::move(a);\
   } \
   DenseRaster<double> operator OPERAND( DenseRaster<double>&& a, const DenseRaster<double>& b ) \
  { \
      if ( !a.can_perform_operation( b ) ) \
      {\
         throw std::invalid_argument( "Cannot Multiple disimilar rasters" ); \
      }\
      std::transform(\
         EXEC_POLICY, a.data.begin(), a.data.end(), b.data.begin(), a.data.begin(),\
         []( const double& a, const double& b )\
         {\
               return a OPERAND b;\
         } );\
      for ( size_t i = 0; i < a.get_ndata(); i++ )\
      {\
         a.nodata_mask[i] = a.nodata_mask[i] || b.nodata_mask[i];\
      }\
      a.set_name( a.get_name() + " " + #OPERAND + " " +b.get_name() );\
      return std::move(a);\
   }\
   DenseRaster<double> operator OPERAND( const DenseRaster<double>& a, DenseRaster<double>&& b ) \
  { \
      if ( !a.can_perform_operation( b ) ) \
      {\
         throw std::invalid_argument( "Cannot Multiple disimilar rasters" ); \
      }\
      std::transform(\
         EXEC_POLICY, a.data.begin(), a.data.end(), b.data.begin(), b.data.begin(),\
         []( const double& a, const double& b )\
         {\
               return a OPERAND b;\
         } );\
      for ( size_t i = 0; i < b.get_ndata(); i++ )\
      {\
         b.nodata_mask[i] = a.nodata_mask[i] || b.nodata_mask[i];\
      }\
      b.set_name( a.get_name() + " " + #OPERAND + " " +b.get_name() );\
      return std::move(b);\
   }\
   DenseRaster<double> operator OPERAND2 ( DenseRaster<double>& a, const DenseRaster<double>& b ) \
   { \
      if ( !a.can_perform_operation( b ) ) \
      {\
         throw std::invalid_argument( "Cannot Multiple disimilar rasters" ); \
      }\
      std::transform(\
         EXEC_POLICY, a.data.begin(), a.data.end(), b.data.begin(), a.data.begin(),\
         []( const double& a, const double& b )\
         {\
               return a OPERAND b;\
         } );\
      for ( size_t i = 0; i < a.get_ndata(); i++ )\
      {\
         a.nodata_mask[i] = a.nodata_mask[i] || b.nodata_mask[i];\
      }\
      a.set_name( a.get_name() + " " + #OPERAND + " " +b.get_name() );\
      return a;\
   }





#define SINGLE_OPERATOR(OP, OPERAND2) \
  DenseRaster<double> operator OP (const double k, const DenseRaster<double>& a) \
  {\
      DenseRaster<double> out( std::make_tuple( a.get_rows(), a.get_cols(), a.get_zindex() ));\
      out.copy_metadata_from(a);\
      std::transform( EXEC_POLICY, a.data.begin(), a.data.end(), out.data.begin(), [&k](double v) { return k OP v;  } );\
      out.nodata_mask = a.nodata_mask;\
      return out;\
  }\
  DenseRaster<double> operator OP(const double k, DenseRaster<double>&& a)\
  {\
      std::transform( EXEC_POLICY, a.data.begin(), a.data.end(), a.data.begin(), [&k](double v) { return k OP v;  } );\
      return std::move(a);\
  }\
  DenseRaster<double> operator OP (const DenseRaster<double>& a, const double k)\
  {\
      DenseRaster<double> out( std::make_tuple( a.get_rows(), a.get_cols(), a.get_zindex() ));\
      out.copy_metadata_from(a);\
      std::transform( EXEC_POLICY, a.data.begin(), a.data.end(), out.data.begin(), [&k](double v) { return v OP k;  } );\
      out.nodata_mask = a.nodata_mask;\
      return out;\
  }\
  DenseRaster<double> operator OP (DenseRaster<double>&& a, const double k)\
  {\
      std::transform( EXEC_POLICY, a.data.begin(), a.data.end(), a.data.begin(), [&k](double v) { return v OP k;  } );\
      return std::move(a);\
  }\
   DenseRaster<double> operator OPERAND2 ( DenseRaster<double>& a, const double k ) \
   {\
      std::transform( EXEC_POLICY, a.data.begin(), a.data.end(), a.data.begin(), [&k](double v) { return v OP k;  } );\
      return a;\
  }\









namespace KiLib::Rasters
{

  OPERATOR(*, *=);

  OPERATOR(-, -=);

  OPERATOR(+, +=);

  OPERATOR(/, /=);

  SINGLE_OPERATOR(*, *=);
  SINGLE_OPERATOR(-, -=);
  SINGLE_OPERATOR(+, +=);
  SINGLE_OPERATOR(/, /=);




   DenseRaster<double> operator-= ( const DenseRaster<double>& a, DenseRaster<double>& b ) 
   { 
      if ( !a.can_perform_operation( b ) ) 
      {
         throw std::invalid_argument( "Cannot Multiple disimilar rasters" ); 
      }

      std::transform(
         EXEC_POLICY, a.data.begin(), a.data.end(), b.data.begin(), b.data.begin(),
         []( const double& a, const double& b )
         {
               return a - b;
         } );

      for ( size_t i = 0; i < b.get_ndata(); i++ )
      {
         b.nodata_mask[i] = a.nodata_mask[i] || b.nodata_mask[i];
      }
      b.set_name( a.get_name() + " - " +b.get_name() );
      return b;
   }




   DenseRaster<double> operator/= ( const DenseRaster<double>& a, DenseRaster<double>& b ) 
   { 
      if ( !a.can_perform_operation( b ) ) 
      {
         throw std::invalid_argument( "Cannot Multiple disimilar rasters" ); 
      }
      std::transform(
         EXEC_POLICY, a.data.begin(), a.data.end(), b.data.begin(), b.data.begin(),
         []( const double& a, const double& b )
         {
               return a / b;
         } );
 
      for ( size_t i = 0; i < b.get_ndata(); i++ )
      {
         b.nodata_mask[i] = a.nodata_mask[i] || b.nodata_mask[i];
      }
      b.set_name( a.get_name() + " / " +b.get_name() );
      return b;
   }


} // namespace KiLib::Rasters
