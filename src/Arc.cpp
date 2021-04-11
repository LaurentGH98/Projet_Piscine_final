#include "Arc.h"

///constructeur
Arc::Arc(int num, std::string nom, std::string type,Sommet* ext1, Sommet* ext2)
{
    m_numero=num;
    m_nom=nom;
    m_type=type;
    m_SommetAdjacent=std::make_pair(ext1,ext2);

    if (ext1->getAltitude()>ext2->getAltitude())
    {
        m_denivele=ext1->getAltitude()-ext2->getAltitude();
    }
    else
    {
        m_denivele=ext2->getAltitude()-ext1->getAltitude();
    }

    if(m_type == "V") {
         m_duree=m_denivele*(5*60)/100;
    }
    else if(m_type == "B") {
         m_duree=m_denivele*(4*60)/100;
    }
    else if(m_type == "R") {
         m_duree=m_denivele*(3*60)/100;
    }
    else if(m_type == "N") {
         m_duree=m_denivele*(2*60)/100;
    }
    else if(m_type == "KL") {
         m_duree=10;
    }
    else if(m_type == "SURF") {
         m_duree=m_denivele*(10*60)/100;
    }
    else if(m_type == "TPH") {
         m_duree=m_denivele*(2*60)/100+(4*60);
    }
    else if(m_type == "TC") {
         m_duree=m_denivele*(3*60)/100+(2*60);
    }
    else if(m_type == "TSD") {
         m_duree=m_denivele*(3*60)/100+(1*60);
    }
    else if(m_type == "TS") {
         m_duree=m_denivele*(4*60)/100+(1*60);
    }else if(m_type == "TK") {
         m_duree=m_denivele*(4*60)/100+(1*60);
    }
    else if(m_type == "BUS") {
        if (m_nom=="navette1600-1800" || m_nom=="navette1800-1600")
        {
            m_duree=30*60;
        }
        else
            m_duree=40*60;
    }


}

///destructeur
Arc::~Arc()
{
}

///getters
int Arc::getDuree()const
{
    return m_duree;
}


std::string Arc::getNom()const
{
    return m_nom;
}

int Arc::getNumero()const
{
    return m_numero;
}

std::string Arc::getType()const
{
    return m_type;
}

std::pair<Sommet*,Sommet*> Arc::getSommetAdj()const
{
    return m_SommetAdjacent;
}

float Arc::getDenivele() const
{
    return m_denivele;
}

int Arc::getOrientation() const
{
    return m_orientation;
}

///setter(s)

void Arc::setOrientation(int orientation)
{
    m_orientation=orientation;
}

///méthode(s)
void Arc::afficher()const
{
    int minutes=m_duree/60;
    int secondes=m_duree%60;
    std::cout<<m_SommetAdjacent.first->getNumero()<<"-"<<m_SommetAdjacent.second->getNumero()<<" : "<<minutes<<" min "<<secondes<<" secondes"<<std::endl;
}
