/*
 * DoubleValidator.h
 *
 *  Created on: 23.10.2009
 *      Author: mars
 */

#ifndef DOUBLEVALIDATOR_H_
#define DOUBLEVALIDATOR_H_

#include <QDoubleValidator>

class CDoubleValidator : public QDoubleValidator
{
public:
   CDoubleValidator( double aMin, double aMax, int aDidgit, QObject* aParent );
   
   virtual QValidator::State validate( QString& anInput, int& aPos )const;
};

#endif /* DOUBLEVALIDATOR_H_ */
