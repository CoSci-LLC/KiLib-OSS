
#include <KiLib/Rasters/SparseRaster_Vector.hpp>



#define OPERATOR(OPERAND)   \
   SparseRaster<double> operator OPERAND( const SparseRaster<double>& a, const SparseRaster<double>& b ) \
   {  \
      if ( !a.can_perform_operation( b ) ) \
      { \
         throw std::invalid_argument( "Cannot Multiple disimilar rasters" ); \
      } \
      KiLib::Rasters::SparseRaster<double> out( a );   \
      const auto                           nodata   = a.get_nodata_value();   \
      const auto                           nodata_b = b.get_nodata_value();    \
      std::transform(    \
         EXEC_POLICY, a.V.begin(), a.V.end(), b.V.begin(), out.V.begin(), \
         [&nodata, &nodata_b]( const double& a, const double& b ) \
         { \
            if ( a == nodata || b == nodata_b ) \
            { \
               return nodata_b; \
            } \
  else \
            { \
               return a OPERAND b; \
            } \
         } ); \
      out.set_name( a.get_name() + " OPERAND " + b.get_name() ); \
      return out; \
   } \
   SparseRaster<double> operator OPERAND( SparseRaster<double>&& a, SparseRaster<double>&& b ) \
   { \
      if ( !a.can_perform_operation( b ) ) \
      {\
         throw std::invalid_argument( "Cannot Multiple disimilar rasters" ); \
      }\
\
      const auto nodata   = a.get_nodata_value();\
      const auto nodata_b = b.get_nodata_value();\
      std::transform(\
         EXEC_POLICY, a.V.begin(), a.V.end(), b.V.begin(), a.V.begin(),\
         [&nodata, &nodata_b]( const double& a, const double& b )\
         {\
            if ( a == nodata || b == nodata_b )\
            {\
               return nodata_b;\
            }\
            else\
            {\
               return a OPERAND b;\
            }\
         } );\
      a.set_name( a.get_name() + " OPERAND " + b.get_name() );\
      return std::move( a );\
   } \
   SparseRaster<double> operator OPERAND( SparseRaster<double>&& a, const SparseRaster<double>& b ) \
  { \
      if ( !a.can_perform_operation( b ) ) \
      {\
         throw std::invalid_argument( "Cannot Multiple disimilar rasters" ); \
      }\
\
      const auto nodata   = a.get_nodata_value();\
      const auto nodata_b = b.get_nodata_value();\
      std::transform(\
         EXEC_POLICY, a.V.begin(), a.V.end(), b.V.begin(), a.V.begin(),\
         [&nodata, &nodata_b]( const double& a, const double& b )\
         {\
            if ( a == nodata || b == nodata_b )\
            {\
               return nodata_b;\
            }\
            else\
            {\
               return a OPERAND b;\
            }\
         } );\
      a.set_name( a.get_name() + " OPERAND " + b.get_name() );\
      return std::move( a );\
   }\
   SparseRaster<double> operator OPERAND( const SparseRaster<double>& a, SparseRaster<double>&& b ) \
  { \
      if ( !a.can_perform_operation( b ) ) \
      {\
         throw std::invalid_argument( "Cannot Multiple disimilar rasters" ); \
      }\
\
      const auto nodata   = a.get_nodata_value();\
      const auto nodata_b = b.get_nodata_value();\
      std::transform(\
         EXEC_POLICY, a.V.begin(), a.V.end(), b.V.begin(), b.V.begin(),\
         [&nodata, &nodata_b]( const double& a, const double& b )\
         {\
            if ( a == nodata || b == nodata_b )\
            {\
               return nodata_b;\
            }\
            else\
            {\
               return a OPERAND b;\
            }\
         } );\
      b.set_name( a.get_name() + " OPERAND " + b.get_name() );\
      return std::move( b );\
   }\


#define SINGLE_OPERATOR(OP) \
  SparseRaster<double> operator OP (const double k, const SparseRaster<double>& a) \
  {\
      KiLib::Rasters::SparseRaster<double> out(a);\
      const auto nodata = a.get_nodata_value();\
      std::transform( EXEC_POLICY, a.V.begin(), a.V.end(), out.V.begin(), [&nodata, &k](double v) { if (v == nodata) {return nodata; } else { return k OP v;  } } );\
      return out;\
  }\
  SparseRaster<double> operator OP(const double k, SparseRaster<double>&& a)\
  {\
      const auto nodata = a.get_nodata_value();\
      std::transform( EXEC_POLICY, a.V.begin(), a.V.end(), a.V.begin(), [&nodata, &k](double v) { if (v == nodata) {return nodata; } else { return k OP v;  } } );\
      return std::move(a);\
  }\
  SparseRaster<double> operator OP (const SparseRaster<double>& a, const double k)\
  {\
      KiLib::Rasters::SparseRaster<double> out(a);\
      const auto nodata = a.get_nodata_value();\
      std::transform( EXEC_POLICY, a.V.begin(), a.V.end(), out.V.begin(), [&nodata, &k](double v) { if (v == nodata) {return nodata; } else { return v OP k;  } } );\
      return out;\
  }\
  SparseRaster<double> operator OP (SparseRaster<double>&& a, const double k)\
  {\
      const auto nodata = a.get_nodata_value();\
      std::transform( EXEC_POLICY, a.V.begin(), a.V.end(), a.V.begin(), [&nodata, &k](double v) { if (v == nodata) {return nodata; } else { return v OP k;  } } );\
      return std::move(a);\
  }








namespace KiLib::Rasters
{

  OPERATOR(*);

  OPERATOR(-);

  OPERATOR(+);

  OPERATOR(/);

  SINGLE_OPERATOR(*);
  SINGLE_OPERATOR(-);
  SINGLE_OPERATOR(+);
  SINGLE_OPERATOR(/);


} // namespace KiLib::Rasters
