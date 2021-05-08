#include "common.h"

LIBNET_API
libnet_ptag_t libnet_build_lldp_chassis(const uint8_t subtype,
const uint8_t *const value,const uint8_t value_s, libnet_t *l,
libnet_ptag_t ptag)
{
  assert(value != NULL && "Chassis ID string cannot be a NULL");
  assert((value_s != 0) && "Chassis ID string length must be greater or equal to 1");
  assert(LIBNET_LLDP_TLV_HDR_SIZE == 0x02 && "TLV header size must be a 2 bytes");
  assert(LIBNET_LLDP_SUBTYPE_SIZE == 0x01 && "Subtype field size must be 1 byte");

  uint32_t n, h;
  libnet_pblock_t *p;
  struct libnet_lldp_hdr hdr;
  memset(&hdr, 0, sizeof(struct libnet_lldp_hdr));

  if (l == NULL)
  {
    return (-1);
  }

  if(value == NULL)
  {
    snprintf(l->err_buf, LIBNET_ERRBUF_SIZE,
    "%s(): Chassis ID string is NULL", __func__);
    return (-1);
  }

  if(value_s == 0)
  {
      snprintf(l->err_buf, LIBNET_ERRBUF_SIZE,
      "%s(): Incorrect Chassis ID string length", __func__);
      return (-1);
  }

  /* size of memory block */
  n = h =  LIBNET_LLDP_TLV_HDR_SIZE + /* TLV Header size */
           LIBNET_LLDP_SUBTYPE_SIZE + /* Chassis ID subtype size */
           value_s;                   /* Chassis ID string length */

  LIBNET_LLDP_TLV_SET_TYPE(hdr.tlv_info, LIBNET_LLDP_CHASSIS_ID);
  LIBNET_LLDP_TLV_SET_LEN(hdr.tlv_info, value_s + LIBNET_LLDP_SUBTYPE_SIZE);

  /*
   *  Find the existing protocol block if a ptag is specified, or create
   *  a new one.
   */
  p = libnet_pblock_probe(l, ptag, n, LIBNET_PBLOCK_LLDP_CHASSIS_H);
  if (p == NULL)
  {
    return (-1);
  }

  const uint16_t type_and_len = htons(hdr.tlv_info);
  if (libnet_pblock_append(l, p, &type_and_len, sizeof(type_and_len)) == -1)
  {
    goto bad;
  }

  if (libnet_pblock_append(l, p, &subtype, sizeof(subtype)) == -1)
  {
    goto bad;
  }

  if (libnet_pblock_append(l, p, value, value_s) == -1)
  {
    goto bad;
  }

  return (ptag ? ptag
               : libnet_pblock_update(l, p, h, LIBNET_PBLOCK_LLDP_CHASSIS_H));
bad:
  libnet_pblock_delete(l, p);
  return (-1);
}


LIBNET_API
libnet_ptag_t libnet_build_lldp_port(const uint8_t subtype,
const uint8_t *const value, const uint8_t value_s,
libnet_t *l, libnet_ptag_t ptag)
{
  assert(value != NULL && "Port ID string cannot be a NULL");
  assert((value_s != 0) && "Port ID string length must be greater or equal to 1");
  assert(LIBNET_LLDP_TLV_HDR_SIZE == 0x02 && "TLV header size must be a 2 bytes");
  assert(LIBNET_LLDP_SUBTYPE_SIZE == 0x01 && "Subtype field size must be 1 byte");

  uint32_t n, h;
  libnet_pblock_t *p;
  struct libnet_lldp_hdr hdr;
  memset(&hdr, 0, sizeof(struct libnet_lldp_hdr));

  if (l == NULL)
  {
    return (-1);
  }

  if(value == NULL)
  {
    snprintf(l->err_buf, LIBNET_ERRBUF_SIZE,
    "%s(): Port ID string is NULL", __func__);
    return (-1);
  }

  if(value_s == 0)
  {
      snprintf(l->err_buf, LIBNET_ERRBUF_SIZE,
      "%s(): Incorrect Port ID string length", __func__);
      return (-1);
  }

  /* size of memory block */
  n = h =  LIBNET_LLDP_TLV_HDR_SIZE + /* TLV Header size */
           LIBNET_LLDP_SUBTYPE_SIZE + /* Port ID subtype size */
           value_s;                   /* Port ID string length */

  LIBNET_LLDP_TLV_SET_TYPE(hdr.tlv_info, LIBNET_LLDP_PORT_ID);
  LIBNET_LLDP_TLV_SET_LEN(hdr.tlv_info, value_s + LIBNET_LLDP_SUBTYPE_SIZE);

  /*
   *  Find the existing protocol block if a ptag is specified, or create
   *  a new one.
   */
  p = libnet_pblock_probe(l, ptag, n, LIBNET_PBLOCK_LLDP_PORT_H);
  if (p == NULL)
  {
    return (-1);
  }

  const uint16_t type_and_len = htons(hdr.tlv_info);
  if (libnet_pblock_append(l, p, &type_and_len, sizeof(type_and_len)) == -1)
  {
    goto bad;
  }

  if (libnet_pblock_append(l, p, &subtype, sizeof(subtype)) == -1)
  {
    goto bad;
  }

  if (libnet_pblock_append(l, p, value, value_s) == -1)
  {
    goto bad;
  }

  return (ptag ? ptag
               : libnet_pblock_update(l, p, h, LIBNET_PBLOCK_LLDP_PORT_H));
bad:
  libnet_pblock_delete(l, p);
  return (-1);
}

LIBNET_API
libnet_ptag_t libnet_build_lldp_ttl(const uint16_t ttl,
libnet_t *l, libnet_ptag_t ptag)
{
  assert(ttl <= UINT16_MAX && "Incorrect value of ttl");
  assert(LIBNET_LLDP_TLV_HDR_SIZE == 0x02 && "TLV header size must be a 2 bytes");
  assert(LIBNET_LLDP_SUBTYPE_SIZE == 0x01 && "Subtype field size must be 1 byte");

  uint32_t n, h;
  libnet_pblock_t *p;
  struct libnet_lldp_hdr hdr;
  memset(&hdr, 0, sizeof(struct libnet_lldp_hdr));

  if (l == NULL)
  {
    return (-1);
  }

  if(ttl > UINT16_MAX)
  {
    snprintf(l->err_buf, LIBNET_ERRBUF_SIZE,
    "%s(): Incorrect value of ttl", __func__);
    return (-1);
  }

  /* size of memory block */
  n = h =  LIBNET_LLDP_TLV_HDR_SIZE + /* TLV Header size */
           sizeof(uint16_t);          /* Size of 2 octets */

  LIBNET_LLDP_TLV_SET_TYPE(hdr.tlv_info, LIBNET_LLDP_TTL);
  LIBNET_LLDP_TLV_SET_LEN(hdr.tlv_info, sizeof(uint16_t)); /* Size is 2 octets */

  /*
   *  Find the existing protocol block if a ptag is specified, or create
   *  a new one.
   */
  p = libnet_pblock_probe(l, ptag, n, LIBNET_PBLOCK_LLDP_TTL_H);
  if (p == NULL)
  {
    return (-1);
  }

  const uint16_t type_and_len = htons(hdr.tlv_info);
  if (libnet_pblock_append(l, p, &type_and_len, sizeof(type_and_len)) == -1)
  {
    goto bad;
  }

  if (libnet_pblock_append(l, p, &ttl, sizeof(ttl)) == -1)
  {
    goto bad;
  }

  return (ptag ? ptag
               : libnet_pblock_update(l, p, h, LIBNET_PBLOCK_LLDP_TTL_H));
bad:
  libnet_pblock_delete(l, p);
  return (-1);
}


LIBNET_API
libnet_ptag_t libnet_build_lldp_end(libnet_t *l, libnet_ptag_t ptag)
{
  assert(LIBNET_LLDP_TLV_HDR_SIZE == 0x02 && "TLV header size must be a 2 bytes");
  assert(LIBNET_LLDP_SUBTYPE_SIZE == 0x01 && "Subtype field size must be 1 byte");

  uint32_t n, h;
  libnet_pblock_t *p;
  struct libnet_lldp_hdr hdr;
  memset(&hdr, 0, sizeof(struct libnet_lldp_hdr));

  if (l == NULL)
  {
    return (-1);
  }

  /* size of memory block */
  n = h =  LIBNET_LLDP_TLV_HDR_SIZE; /* TLV Header size */

  LIBNET_LLDP_TLV_SET_TYPE(hdr.tlv_info, LIBNET_LLDP_END_LLDPDU);
  LIBNET_LLDP_TLV_SET_LEN(hdr.tlv_info, 0);

  /*
   *  Find the existing protocol block if a ptag is specified, or create
   *  a new one.
   */
  p = libnet_pblock_probe(l, ptag, n, LIBNET_PBLOCK_LLDP_TTL_H);
  if (p == NULL)
  {
    return (-1);
  }

  const uint16_t type_and_len = htons(hdr.tlv_info);
  if (libnet_pblock_append(l, p, &type_and_len, sizeof(type_and_len)) == -1)
  {
    goto bad;
  }

  return (ptag ? ptag
               : libnet_pblock_update(l, p, h, LIBNET_PBLOCK_LLDP_TTL_H));
bad:
  libnet_pblock_delete(l, p);
  return (-1);
}

LIBNET_API
libnet_ptag_t libnet_build_lldp_org_spec(const uint8_t *const value,
const uint16_t value_s, libnet_t *l, libnet_ptag_t ptag)
{
  uint32_t n, h;
  libnet_pblock_t *p;
  struct libnet_lldp_hdr hdr;
  memset(&hdr, 0, sizeof(struct libnet_lldp_hdr));

  if (l == NULL) {
    return (-1);
  }

  if(value == NULL)
  {
    snprintf(l->err_buf, LIBNET_ERRBUF_SIZE,
    "%s(): Organization Specific string is NULL", __func__);
    return (-1);
  }

  if((value_s < 4) || (value_s > 511))
  {
      snprintf(l->err_buf, LIBNET_ERRBUF_SIZE,
      "%s(): Incorrect TLV information string length", __func__);
      return (-1);
  }

  LIBNET_LLDP_TLV_SET_TYPE(hdr.tlv_info, LIBNET_LLDP_ORG_SPEC);
  LIBNET_LLDP_TLV_SET_LEN(hdr.tlv_info, value_s);

  /* size of memory block */
  n = h = LIBNET_LLDP_TLV_HDR_SIZE + /* TLV Header size */
          value_s;                   /* TLV Information length*/

  /*
   *  Find the existing protocol block if a ptag is specified, or create
   *  a new one.
   */
  p = libnet_pblock_probe(l, ptag, n, LIBNET_PBLOCK_LLDP_ORG_SPEC_H);
  if (p == NULL) {
    return (-1);
  }

  const uint16_t type_and_len = htons(hdr.tlv_info);
  if (libnet_pblock_append(l, p, &type_and_len, sizeof(type_and_len)) == -1)
  {
    goto bad;
  }

  if (libnet_pblock_append(l, p, value, value_s) == -1) {
    goto bad;
  }

  return (ptag ? ptag : libnet_pblock_update(l, p, h, LIBNET_PBLOCK_LLDP_ORG_SPEC_H));
bad:
  libnet_pblock_delete(l, p);
  return (-1);
}

/**
 * Local Variables:
 *  indent-tabs-mode: nil
 *  c-file-style: "stroustrup"
 * End:
 */
